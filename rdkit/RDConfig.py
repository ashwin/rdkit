#  $Id$
#
#  Copyright (C) 2000-2006 Rational Discovery LLC
#
#   @@ All Rights Reserved  @@
#
""" Configuration for Rational Discovery's Python code

"""

import os,sys
try:
  RDBaseDir=os.environ['RDBASE']
except:
  RDBaseDir='d:/RD'  
RDCodeDir=os.path.join(RDBaseDir,'rdkit')
RDImageDir=os.path.join(RDCodeDir,'qtGui','Images')  # images used within the GUI tools
RDDataDir=os.path.join(RDBaseDir,'Data')
RDDocsDir=os.path.join(RDBaseDir,'Docs')
RDDemoDir=os.path.join(RDBaseDir,'Demo')
RDBinDir=os.path.join(RDBaseDir,'bin')
RDProjDir=os.path.join(RDBaseDir,'Projects')
#if not os.environ.has_key('OE_DIR') or os.environ['OE_DIR']=='':
#  os.environ['OE_DIR'] = RDDataDir


if os.environ.has_key('RDDGEOM'):
  dgeomLoc=os.environ['RDDGEOM']
else:
  if sys.platform=='win32':
    binName = 'dgeom.exe'
  else:
    binName = 'dgeom'
  dgeomLoc=os.path.join(RDBinDir,binName)

if os.environ.has_key('RDTINKER'):
  tinkerPath=os.environ['RDTINKER']
else:
  if sys.platform=='win32':
    tinkerPath = ''
  else:
    tinkerPath = '/usr/local/src/tinker'
tinkerBin = '%s/bin'%(tinkerPath)
tinkerParams = '%s/params'%(tinkerPath)

try:
  doingDemo=os.environ['RDDEMO']
  if doingDemo=='0': doingDemo=0
except:
  doingDemo=0

try:
  largeFont=os.environ['RDLARGEFONT']
except:
  largeFont=0

RDFont=None
logGui=0

rpcTestPort=8423
pythonTestCommand="python"

defaultDBUser='sysdba'
defaultDBPassword='masterkey'


import exceptions
class ObsoleteCodeError(exceptions.Exception):
  pass
class UnimplementedCodeError(exceptions.Exception):
  pass

# ---------------------
# the following block contains stuff used by the
# testing infrastructure
if sys.platform=='win32':
  pythonExe=os.path.join(os.environ.get('PYTHONHOME','c:/python25'),'python.exe')
  spawn = os.spawnve
else:
  pythonExe="python"
  spawn = os.spawnvpe

# ---------------------
# the following block contains stuff controlling database access:
usePgSQL=False
useSqlLite=False
if not os.environ.get('RD_USESQLLITE',''):
  try:
    from pyPgSQL import PgSQL
    usePgSQL=True
  except ImportError:
    usePgSQL=False
if not usePgSQL:
  try:
    # python2.5 has this:
    import sqlite3
    useSqlLite=True
  except ImportError:
    try:
      # earlier versions of python:
      from pysqlite2 import dbapi2
      useSqlLite=True
    except:
      pass

if usePgSQL:
  RDTestDatabase='::RDTests'
  RDDataDatabase='::RDData'
elif useSqlLite:
  RDTestDatabase=os.path.join(RDDataDir,"RDTests.sqlt")
  RDDataDatabase=os.path.join(RDDataDir,"RDData.sqlt")
else:  
  RDTestDatabase=None
  RDDataDatabase=None

# ---------------------
# the following block contains stuff controlling the program used for
#  3D molecular visualization:
molViewer=os.environ.get('RD_MOLVIEWER','PYMOL').upper()

  
