#----------------------------------------------------------------------
# Name:        
# Purpose:     a qtWidget derived class that can hold an ActiveX control
# Author:      Greg Landrum
#
# Copyright:   (c) 2003 by Rational Discovery LLC
# Licence:     wxWindows license
#
# Acknowledgements: derived from wxPython.lib.activexwrapper (part of wxPython)
#----------------------------------------------------------------------

from qt import *
import sys
if sys.platform == 'win32':
  try:
    import win32ui
    import pywin.mfc.activex
    import win32com.client
  except ImportError:
    raise ImportError( "qtActiveX requires the PythonWin extensions.")
else:
  raise ImportError("ActiveX support only available under Windows")

from win32con import WS_TABSTOP, WS_VISIBLE, WS_CHILD

#----------------------------------------------------------------------

def MakeActiveXClass(CoClass, eventClass=None, eventObj=None):
  """
  Dynamically construct a new class that derives from QWidget, the
  ActiveX control and the appropriate COM classes.  This new class
  can be used just like the Qt class, but will also respond
  appropriately to the methods and properties of the COM object.  If
  this class, a derived class or a mix-in class has method names
  that match the COM object's event names, they will be called
  automatically.
  
  CoClass -- A COM control class from a module generated by
    makepy.py from a COM TypeLibrary.  Can also accept a
    CLSID.
  
  eventClass -- If given, this class will be added to the set of
      base classes that the new class is drived from.  It is
      good for mix-in classes for catching events.

  eventObj -- If given, this object will be searched for attributes
      by the new class's __getattr__ method, (like a mix-in
      object.)  This is useful if you want to catch COM
      callbacks in an existing object, (such as the parent
      window.)
      
  """
  if type(CoClass) == type(""):
    # use the CLSID to get the real class
    CoClass = win32com.client.CLSIDToClass(CoClass)

  # determine the base classes
  axEventClass = CoClass.default_source
  baseClasses = [QWidget, pywin.mfc.activex.Control, CoClass, axEventClass]
  if eventClass:
    baseClasses.append(eventClass)
  baseClasses = tuple(baseClasses)

  # define the class attributes
  className = 'AXControl_'+CoClass.__name__
  classDict = { '__init__'    : axw__init__,
                '__getattr__' : axw__getattr__,
                '_name'       : className,
                '_eventBase'  : axEventClass,
                '_eventObj'   : eventObj,
                }
  # make a new class object
  import new
  classObj = new.classobj(className, baseClasses, classDict)
  return classObj

# These functions will be used as methods in the new class
def axw__init__(self, parent, name='', fl=0):
  # init base classes
  pywin.mfc.activex.Control.__init__(self)
  QWidget.__init__(self, parent, name, 0)

  win32ui.EnableControlContainer()
  self._eventObj = self._eventObj  # move from class to instance

  # create a pythonwin wrapper around this widget
  handle = parent.winId()
  self._wnd = win32ui.CreateWindowFromHandle(handle)
  # create the control
  sz = parent.size()
  self.CreateControl(self._name, WS_TABSTOP | WS_VISIBLE | WS_CHILD,
                     (0, 0, sz.width(), sz.height()), self._wnd, -1)
  # init the ax events part of the object
  self._eventBase.__init__(self, self._dispobj_)

def axw__getattr__(self, attr):
  try:
    return pywin.mfc.activex.Control.__getattr__(self, attr)
  except AttributeError:
    try:
      eo = self.__dict__['_eventObj']
      return getattr(eo, attr)
    except AttributeError:
      raise AttributeError('Attribute not found: %s' % attr)




