#pragma once
/*==============================================================================

 medInria

 Copyright (c) INRIA 2022. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

==============================================================================*/

#include <QString>
#include "medPythonExport.h"

namespace med::python
{

struct PythonManagerPrivate;

class MEDPYTHON_EXPORT PythonManager
{
public:
    static const int consoleWidth = 800;
    static const int consoleHeight = 600;
    static constexpr char consoleTitle[] = "Python console";
    static constexpr char consoleShortCut[] = "Ctrl+Shift+P"; // Ctrl maps to the command key on macOS

    PythonManager();
    ~PythonManager();

    bool pythonIsUsable();

    void loadPlugins();
    void startConsole();

private:
    PythonManagerPrivate* const d;

    void initialize();
    void initializeInterpreter();
    void initializeAPI();
    void initializePaths();
    void initializeTools();

    void finalize();
    void finalizeInterpreter();

    QStringList getInternalPaths(QStringList pathDefinitions);
    bool testInternalPaths(QStringList paths);
};

} // namespace med::python
