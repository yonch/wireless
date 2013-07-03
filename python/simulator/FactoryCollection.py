# Copyright (c) 2012 Jonathan Perry
# This code is released under the MIT license (see LICENSE file).

##
# \ingroup simulator
# \brief Aggregates multiple factories to be queried jointly for components 
class FactoryCollection(object):
    """
    This class is intended to make adding new functionality modular through adding factories.
        A factory collection maintains several factory objects, and has a factory method make().
        When using make(), the caller passes a name of a factory function. This factory function is 
        called in all factories that contain a function with that name, until one of them returns
        non-null.
        
    The collection maintains a cache of factory results for each factory function name.
    
    self._factories is a dictionary from factory function name to the factories that contain
        that function.
    self._cache is a dictionary from factory function name to a tuple (args, val) where args are 
        the arguments of the cached factory call and val is the object that was produced by the 
        call.
    """
    
    def __init__(self, function_names):
        """
        C'tor
        @param function_names: a list of strings, each a name of a factory function
        """
        self._factories = dict([(name, []) for name in function_names])
        self._cache = dict([(name, (None, None)) for name in function_names])
        
    def add_factory(self, factory):
        # for each factory function name:
        for func_name in self._factories.keys():
            # if factory has an attribute func_name
            if hasattr(factory, func_name):
                # add the factory function into self._factories under the function name
                self._factories[func_name].append(getattr(factory, func_name))
                
    def make(self, func_name, *args):
        # Cache lookup
        cached_args, cached_val = self._cache[func_name]
        if cached_args == args:
            return cached_val
        
        # Call factory functions until a non-null value is returned
        val = None
        for factory_func in self._factories[func_name]:
            val = factory_func(*args)
            if val is not None:
                # update cache
                self._cache[func_name] = (args, val)
                # return produced value
                return val
         
        # Couldn't find a result
        raise RuntimeError, "No '%s' factory produced an object" % func_name
    