// $Id$
//
// Copyright (C) 2003-2006 Greg Landrum and Rational Discovery LLC
//
//   @@ All Rights Reserved  @@
//
#include "QueryOps.h"
#include <algorithm>
#include <RDGeneral/types.h>

namespace RDKit{

  // common general queries


//! returns a Query for matching atoms with a particular number of ring bonds
ATOM_EQUALS_QUERY *makeAtomRingBondCountQuery(int what) {
  ATOM_EQUALS_QUERY *res=new AtomRingQuery(what);
  res->setDescription("AtomRingBondCount");
  res->setDataFunc(queryAtomRingBondCount);
  return res;
};

template <class T,int sz>  
int queryIsInRingOfSize(T const *obj) {
  return obj->getOwningMol().getRingInfo()->isAtomInRingOfSize(obj->getIdx(),sz);
};
ATOM_EQUALS_QUERY *makeAtomInRingOfSizeQuery(int tgt){
  RANGE_CHECK(3,tgt,20);
  ATOM_EQUALS_QUERY *res = new ATOM_EQUALS_QUERY;
  res->setVal(true);
  switch(tgt){
  case 3:
    res->setDataFunc(queryIsInRingOfSize<Atom,3>);break;
  case 4:
    res->setDataFunc(queryIsInRingOfSize<Atom,4>);break;
  case 5:
    res->setDataFunc(queryIsInRingOfSize<Atom,5>);break;
  case 6:
    res->setDataFunc(queryIsInRingOfSize<Atom,6>);break;
  case 7:
    res->setDataFunc(queryIsInRingOfSize<Atom,7>);break;
  case 8:
    res->setDataFunc(queryIsInRingOfSize<Atom,8>);break;
  case 9:
    res->setDataFunc(queryIsInRingOfSize<Atom,9>);break;
  case 10:
    res->setDataFunc(queryIsInRingOfSize<Atom,10>);break;
  case 11:
    res->setDataFunc(queryIsInRingOfSize<Atom,11>);break;
  case 12:
    res->setDataFunc(queryIsInRingOfSize<Atom,12>);break;
  case 13:
    res->setDataFunc(queryIsInRingOfSize<Atom,13>);break;
  case 14:
    res->setDataFunc(queryIsInRingOfSize<Atom,14>);break;
  case 15:
    res->setDataFunc(queryIsInRingOfSize<Atom,15>);break;
  case 16:
    res->setDataFunc(queryIsInRingOfSize<Atom,16>);break;
  case 17:
    res->setDataFunc(queryIsInRingOfSize<Atom,17>);break;
  case 18:
    res->setDataFunc(queryIsInRingOfSize<Atom,18>);break;
  case 19:
    res->setDataFunc(queryIsInRingOfSize<Atom,19>);break;
  case 20:
    res->setDataFunc(queryIsInRingOfSize<Atom,20>);break;
  }
  res->setDescription("AtomRingSize");
  return res;
}
BOND_EQUALS_QUERY *makeBondInRingOfSizeQuery(int tgt){
  RANGE_CHECK(3,tgt,15);
  BOND_EQUALS_QUERY *res = new BOND_EQUALS_QUERY;
  res->setVal(true);
  switch(tgt){
  case 3:
    res->setDataFunc(queryIsInRingOfSize<Bond,3>);break;
  case 4:
    res->setDataFunc(queryIsInRingOfSize<Bond,4>);break;
  case 5:
    res->setDataFunc(queryIsInRingOfSize<Bond,5>);break;
  case 6:
    res->setDataFunc(queryIsInRingOfSize<Bond,6>);break;
  case 7:
    res->setDataFunc(queryIsInRingOfSize<Bond,7>);break;
  case 8:
    res->setDataFunc(queryIsInRingOfSize<Bond,8>);break;
  case 9:
    res->setDataFunc(queryIsInRingOfSize<Bond,9>);break;
  case 10:
    res->setDataFunc(queryIsInRingOfSize<Bond,10>);break;
  case 11:
    res->setDataFunc(queryIsInRingOfSize<Bond,11>);break;
  case 12:
    res->setDataFunc(queryIsInRingOfSize<Bond,12>);break;
  case 13:
    res->setDataFunc(queryIsInRingOfSize<Bond,13>);break;
  case 14:
    res->setDataFunc(queryIsInRingOfSize<Bond,14>);break;
  case 15:
    res->setDataFunc(queryIsInRingOfSize<Bond,15>);break;
  case 16:
    res->setDataFunc(queryIsInRingOfSize<Bond,16>);break;
  case 17:
    res->setDataFunc(queryIsInRingOfSize<Bond,17>);break;
  case 18:
    res->setDataFunc(queryIsInRingOfSize<Bond,18>);break;
  case 19:
    res->setDataFunc(queryIsInRingOfSize<Bond,19>);break;
  case 20:
    res->setDataFunc(queryIsInRingOfSize<Bond,20>);break;
  }
  res->setDescription("BondRingSize");
  return res;
}


template <class T>  
int queryMinRingSize(T const *obj) {
  return obj->getOwningMol().getRingInfo()->minAtomRingSize(obj->getIdx());
};
ATOM_EQUALS_QUERY *makeAtomMinRingSizeQuery(int tgt){
  RANGE_CHECK(3,tgt,20);
  ATOM_EQUALS_QUERY *res = new ATOM_EQUALS_QUERY;
  res->setVal(tgt);
  res->setDataFunc(queryMinRingSize<Atom>);
  res->setDescription("AtomMinRingSize");
  return res;
}
BOND_EQUALS_QUERY *makeBondMinRingSizeQuery(int tgt){
  RANGE_CHECK(3,tgt,20);
  BOND_EQUALS_QUERY *res = new BOND_EQUALS_QUERY;
  res->setVal(tgt);
  res->setDataFunc(queryMinRingSize<Bond>);
  res->setDescription("BondMinRingSize");
  return res;
}



ATOM_EQUALS_QUERY *makeAtomSimpleQuery(int what,int func(Atom const *)){
  ATOM_EQUALS_QUERY *res = new ATOM_EQUALS_QUERY;
  res->setVal(what);
  res->setDataFunc(func);
  res->setDescription("AtomSimple");
  return res;
}


unsigned int queryAtomBondProduct(Atom const * at) {
  ROMol::OEDGE_ITER beg,end;
  ROMol::GRAPH_MOL_BOND_PMAP::type pMap = at->getOwningMol().getBondPMap();
  boost::tie(beg,end) = at->getOwningMol().getAtomBonds(at);
  unsigned int prod=1;
  while(beg!=end){
    const Bond *bond=pMap[*beg];
    prod *= static_cast<unsigned int>(firstThousandPrimes[bond->getBondType()]);
    beg++;
  }
  return prod;
}
unsigned int queryAtomAllBondProduct(Atom const * at) {
  ROMol::OEDGE_ITER beg,end;
  ROMol::GRAPH_MOL_BOND_PMAP::type pMap = at->getOwningMol().getBondPMap();
  boost::tie(beg,end) = at->getOwningMol().getAtomBonds(at);
  unsigned int prod=1;
  while(beg!=end){
    const Bond *bond=pMap[*beg];
    prod *= static_cast<unsigned int>(firstThousandPrimes[bond->getBondType()]);
    beg++;
  }
  for(unsigned int i=0;i<at->getTotalNumHs();i++){
    prod *= static_cast<unsigned int>(firstThousandPrimes[Bond::SINGLE]);
  }
  return prod;
}



ATOM_EQUALS_QUERY *makeAtomImplicitValenceQuery(int what){
  ATOM_EQUALS_QUERY *res = makeAtomSimpleQuery(what,queryAtomImplicitValence);
  res->setDescription("AtomImplicitValence");
  return res;
}
  
ATOM_EQUALS_QUERY *makeAtomTotalValenceQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomTotalValence);
  res->setDescription("AtomTotalValence");
  return res;
}
  
ATOM_EQUALS_QUERY *makeAtomNumEqualsQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomNum);
  res->setDescription("AtomAtomicNum");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomExplicitDegreeQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomExplicitDegree);
  res->setDescription("AtomExplicitDegree");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomTotalDegreeQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomTotalDegree);
  res->setDescription("AtomTotalDegree");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomHCountQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomHCount);
  res->setDescription("AtomHCount");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomAromaticQuery(){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(true,queryAtomAromatic);
  res->setDescription("AtomIsAromatic");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomAliphaticQuery(){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(true,queryAtomAliphatic);
  res->setDescription("AtomIsAliphatic");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomUnsaturatedQuery(){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(true,queryAtomUnsaturated);
  res->setDescription("AtomUnsaturated");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomMassQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomMass);
  res->setDescription("AtomMass");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomFormalChargeQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomFormalCharge);
  res->setDescription("AtomFormalCharge");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomHybridizationQuery(int what){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(what,queryAtomHybridization);
  res->setDescription("AtomHybridization");
  return res;
}
  
ATOM_EQUALS_QUERY *makeAtomInRingQuery(){
  ATOM_EQUALS_QUERY *res=makeAtomSimpleQuery(true,queryIsAtomInRing);
  res->setDescription("AtomInRing");
  return res;
}

ATOM_EQUALS_QUERY *makeAtomInNRingsQuery(int what){
  ATOM_EQUALS_QUERY *res;
  res = makeAtomSimpleQuery(what,queryIsAtomInNRings);
  res->setDescription("AtomInNRings");
  return res;
}

BOND_EQUALS_QUERY *makeBondOrderEqualsQuery(Bond::BondType what){
  BOND_EQUALS_QUERY *res = new BOND_EQUALS_QUERY;
  res->setVal(what);
  res->setDataFunc(queryBondOrder);
  res->setDescription("BondOrder");
  return res;
}

BOND_EQUALS_QUERY *makeBondDirEqualsQuery(Bond::BondDir what){
  BOND_EQUALS_QUERY *res = new BOND_EQUALS_QUERY;
  res->setVal(what);
  res->setDataFunc(queryBondDir);
  res->setDescription("BondDir");
  return res;
}

BOND_EQUALS_QUERY *makeBondIsInRingQuery(){
  BOND_EQUALS_QUERY *res = new BOND_EQUALS_QUERY;
  res->setVal(true);
  res->setDataFunc(queryIsBondInRing);
  res->setDescription("BondInRing");
  return res;
}

BOND_EQUALS_QUERY *makeBondInNRingsQuery(int what){
  BOND_EQUALS_QUERY *res = new BOND_EQUALS_QUERY;
  res->setVal(what);
  res->setDataFunc(queryIsBondInNRings);
  res->setDescription("BondInNRings");
  return res;
}


template <typename T>
int nullDataFun(T arg) { return 1; } 
bool nullQueryFun(int arg) { return true; } 

BOND_NULL_QUERY *makeBondNullQuery(){
  BOND_NULL_QUERY *res = new BOND_NULL_QUERY;
  res->setDataFunc(nullDataFun);
  res->setMatchFunc(nullQueryFun);
  res->setDescription("BondNull");
  return res;
}

ATOM_NULL_QUERY *makeAtomNullQuery(){
  ATOM_NULL_QUERY *res = new ATOM_NULL_QUERY;
  res->setDataFunc(nullDataFun);
  res->setMatchFunc(nullQueryFun);
  res->setDescription("AtomNull");
  return res;
}
  
};
