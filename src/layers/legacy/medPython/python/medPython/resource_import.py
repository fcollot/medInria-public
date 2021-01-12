import sys
import importlib.machinery

PathEntryFinder = importlib.machinery.FileFinder

class ResourceFinder(importlib.machinery.FileFinder):

    @classmethod
    def path_hook(cls, *loader_details):
        """A class method which returns a closure to use on sys.path_hook
        which will return an instance using the specified loaders and the path
        called on the closure.
        If the path called on the closure is not a resource, ImportError is
        raised.
        """
        def path_hook_for_ResourceFinder(path):
            """Path hook for ResourceFinder."""
            print("path_hook")
            if not path.startswith(":/python"):
                raise ImportError('resource does not exist', path=path)
            return cls(path, *loader_details)

        return path_hook_for_ResourceFinder

    def find_spec(self, fullname, target=None):
        print(fullname)
        return super().find_spec(fullname, target)

loader_details = (importlib.machinery.SourceFileLoader, importlib.machinery.SOURCE_SUFFIXES)
sys.path_hooks.insert(0, ResourceFinder.path_hook(loader_details))
