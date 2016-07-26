#ifndef PENINTERFACES_HPP
#define PENINTERFACES_HPP
#include <deque>
#include <string>
#include <iostream>
#include <cstddef>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <memory>
#include "suimem.hpp"

using std :: deque;
using std :: string;
using std :: istream;
using std :: ostream;
using std :: stack;
using std :: unordered_map;
using std :: unordered_set;
using std :: pair;
using std :: shared_ptr;
using namespace sui;

class TScanner;
class TParser;
class TFile;
class TError;
class Package;
extern TError Error;
extern TScanner Scanner;
extern TParser Parser;
extern TFile File;

typedef deque<Package> TSeq_arg;
typedef pair<int, int> block;
typedef unordered_map<long long, unordered_map<long long, Package>> TState;
typedef unordered_map<string, int> TPro_table;

const size_t _round_bracket_l   = 0;
const size_t _round_bracket_r   = 1;
const size_t _rect_bracket_l    = 2;
const size_t _rect_bracket_r    = 3;
const size_t _italian_bracket_l = 4;
const size_t _italian_bracket_r = 5;

#endif // PENINTERFACES_HPP
