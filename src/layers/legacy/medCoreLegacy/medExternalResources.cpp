#include "medExternalResources.h"

#if defined(Q_OS_MACOS)
#include <CoreFoundation/CFBundle.h>
#endif

#include <QApplication>
#include <QDir>

namespace med
{

namespace
{

// Search for the resource in the 'resources/[libraryName/]' directory.
QString getResourcePathFromBaseDirectory(QString filename, QString libraryName, QDir directory)
{
    QString result;

    if (directory.cd("resources")
        && (libraryName.isEmpty() || directory.cd(libraryName)))
    {
        QString filePath = directory.filePath(filename);

        if (QFileInfo::exists(filePath))
        {
            result = filePath;
        }
    }

    return result;
}

// On windows the package should have the following structure:
//
// package_root/
//   bin/
//     program.exe
//   resources/
//     main_resource_files
//     libraryName/
//       library_resource_files
//
#if defined(Q_OS_WIN)

QString getResourcePathForWindowsPackage(QString filename, QString libraryName)
{
    QString result;
    QString applicationPath = qApp->applicationDirPath();

    if (!applicationPath.isEmpty())
    {
        QDir medInriaDirectory = applicationPath;

        if (medInriaDirectory.cdUp())
        {
            result = getResourcePathFromBaseDirectory(filename, libraryName, medInriaDirectory);
        }
    }

    return result;
}

#endif

// On macOS the main resources are located in the bundle's resource folder and
// the library-specific resources are located in the library's framework
// resource folder. The bundle and framework structures are accessed through the
// macOS API.
#if defined(Q_OS_MACOS)

// Library frameworks are accessed as bundles.
CFBundleRef getBundle(QString libraryName)
{
    CFBundleRef bundle;

    if (libraryName.isEmpty())
    {
        bundle = CFBundleGetMainBundle();
    }
    else
    {
        QString frameworkId = QString("medInria_IDENTIFIER.%1").arg(libraryName);
        CFStringRef frameworkIdRef = CFStringCreateWithCString(nullptr, qUtf8Printable(frameworkId), kCFStringEncodingUTF8);
        bundle = CFBundleGetBundleWithIdentifier(frameworkIdRef);
        CFRelease(frameworkIdRef);
    }

    return bundle;
}

QString getResourcePathForMacPackage(QString filename, QString libraryName)
{
    QString result;
    CFBundleRef bundle = getBundle(libraryName);

    if (bundle)
    {
        CFStringRef resourceName = CFStringCreateWithCString(nullptr, qUtf8Printable(filename), kCFStringEncodingUTF8);
        CFURLRef resourceURL = CFBundleCopyResourceURL(bundle, resourceName, nullptr, nullptr);
        CFRelease(bundle);
        CFRelease(resourceName);

        if (resourceURL)
        {
            CFStringRef resourcePath = CFURLCopyFileSystemPath(resourceURL, kCFURLPOSIXPathStyle);
            CFRelease(resourceURL);
            CFIndex utf16length = CFStringGetLength(resourcePath);
            CFIndex maxUtf8length = CFStringGetMaximumSizeForEncoding(utf16length, kCFStringEncodingUTF8);
            std::string pathString(maxUtf8length, '\0');

            if (CFStringGetCString(resourcePath, pathString.data(), maxUtf8length, kCFStringEncodingUTF8))
            {
                result = pathString.c_str();
            }

            CFRelease(resourcePath);
        }
    }

    return result;
}

#endif

} // namespace

QString getExternalResourcePath(QString filename, QString libraryName)
{
    QString result;

    // The MEDINRIA_DIR environment variable is defined in the launchers for the
    // build environment and also the linux packages. We can use it to access
    // the resources directory but we ignore it for macOS builds because the
    // resources are always placed in the bundle or frameworks.
#if not defined(Q_OS_MACOS)
    QString medInriaDirectory = qEnvironmentVariable("MEDINRIA_DIR");

    if (!medInriaDirectory.isEmpty())
    {
        result = getResourcePathFromBaseDirectory(filename, libraryName, medInriaDirectory);
    }
#if defined(Q_OS_WIN)
    else
    {
        result = getResourcePathForWindowsPackage(filename, libraryName);
    }
#endif
#else
    result = getResourcePathForMacPackage(filename, libraryName);
#endif

    return result;
}

} // namespace med
