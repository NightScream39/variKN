// Copyright (C) 2007  Vesa Siivola. 
// See licence.txt for the terms of distribution.

// Routines for reading and writing arpa format files from and to the 
// internal prefix tree format.
#ifndef TREEGRAMARPAREADER_HH
#define TREEGRAMARPAREADER_HH

#include <stdio.h>
#include "TreeGram.hh"
#include <map>

class TreeGramArpaReader {
public:
  TreeGramArpaReader();
  void read(FILE *file, TreeGram *tree_gram);
  void write(FILE *file, TreeGram *tree_gram);
  void write_interpolated(FILE *file, TreeGram *treegram);

private:
  void read_error();

  std::vector<int> m_counts;
  int m_lineno;
};

#endif /* TREEGRAMARPAREADER_HH */
