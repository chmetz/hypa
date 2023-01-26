#ifndef PTX_ANALYSER_SRC_DATA_STRUCTURES_GRAPH_H_
#define PTX_ANALYSER_SRC_DATA_STRUCTURES_GRAPH_H_

#include <map>
#include <set>
#include <list>
#include <string>

#include <filesystem>
#include <iostream>
#include <fstream>

template<typename T>
class Graph {
  std::map<T, std::set<T>> adjList_;
  std::map<T, std::set<T>> decList_;
  std::set<T> importantList_;

 public:
  Graph();
  void AddEdge(const T &src, const T &dst);
  void AddImportant(const T &s);
  void ExportDotFile(const std::string &filename) const;
  void PrintRawGraph() const;
  std::set<T> GetAncestorsOfImportant() const;
  std::set<T> GetAncestorsOfImportantFast(std::set<T> &id_set);
  std::set<T> GetImportant() const;
};

template<typename T>
Graph<T>::Graph() = default;

template<typename T>
void Graph<T>::AddEdge(const T &src, const T &dst) {
  adjList_[src].insert(dst);
  decList_[dst].insert(src);
}

template<typename T>
void Graph<T>::AddImportant(const T &s) {
  importantList_.insert(s);
}

template<typename T>
void Graph<T>::ExportDotFile(const std::string &filename) const {
  std::string path(filename);
  size_t pos = path.find_last_of('/');

  if (pos != std::string::npos) {
    path = filename.substr(0, pos);
    if (!exists(std::filesystem::path(path))) {
      std::filesystem::create_directories(path);
    }
  }

  std::stringstream str1;
  std::set<T> to_be_colored(GetAncestorsOfImportant());

  str1 << "digraph {\n";

  str1 << "\tnode [color=red,style=filled]\n";
  for (const auto &s : importantList_) {
    str1 << "\t\"" << s << "\"\n";
  }

  str1 << "\tnode [color=lightblue,style=filled]\n";
  for (const auto &s : to_be_colored) {
    str1 << "\t\"" << s << "\"\n";
  }

  str1 << "\tnode [color=black,style=solid]\n";
  for (const auto &[vertex, adcs] : adjList_) {
    str1 << "\t\"" << vertex << "\" -> {";

    for (const auto &v : adcs) {
      str1 << "\"" << v << "\"";
      if (v != *--adcs.end()) {
        str1 << " ";
      }
    }

    str1 << "}\n";
  }

  str1 << "}\n";

  std::ofstream f1(filename);
  if (f1.is_open()) {
    for (const auto &c : str1.str()) {
      if (c == '%') {
        f1 << "\\";
      }
      f1 << c;
    }
    f1.close();
  } else {
    std::cerr << "Cannot write to file: " << filename << "\n";
  }

  // filtered graph
  std::stringstream str2;
  str2 << "digraph {\n";

  for (const auto &[vertex, adcs] : adjList_) {
    if (to_be_colored.contains(vertex) || importantList_.contains(vertex)) {
      str2 << "\t\"" << vertex << "\" -> {";

      for (const auto &v : adcs) {
        if (to_be_colored.contains(v) || importantList_.contains(v)) {
          str2 << "\"" << v << "\"";
          if (v != *--adcs.end()) {
            str2 << " ";
          }
        }
      }

      str2 << "}\n";
    }
  }

  str2 << "}\n";

  std::ofstream f2(filename + ".filtered.dot");
  if (f2.is_open()) {
    for (const auto &c : str2.str()) {
      if (c == '%') {
        f2 << "\\";
      }
      f2 << c;
    }
    f2.close();
  } else {
    std::cerr << "Cannot write to file: " << filename << ".filtered.dot" << "\n";
  }
}

template<typename T>
void Graph<T>::PrintRawGraph() const {
  std::cout << "--- graph ---\n";

  std::cout << "important nodes\n";
  for (const auto &s : importantList_) {
    std::cout << "  " << s << "\n";
  }

  std::cout << "adjacency list\n";
  for (const auto &[vertex, adcs] : adjList_) {
    std::cout << "  " << vertex << ": ";

    for (const auto &v : adcs) {
      std::cout << v;
      if (v != *--adcs.end()) {
        std::cout << ", ";
      }
    }

    std::cout << "\n";
  }

  std::cout << "descendants list\n";
  for (const auto &[vertex, desc] : decList_) {
    std::cout << "  " << vertex << ": ";

    for (const auto &v : desc) {
      std::cout << v;
      if (v != *--desc.end()) {
        std::cout << ", ";
      }
    }

    std::cout << "\n";
  }

  std::cout << "-------------\n";
}

template<typename T>
std::set<T> Graph<T>::GetAncestorsOfImportant() const {
  std::set<T> to_be_colored(importantList_);
  size_t old_size = 0;

  while (to_be_colored.size() != old_size) {
    old_size = to_be_colored.size();

    for (const auto &s : to_be_colored) {
      auto l = decList_.find(s);
      if (l != decList_.end()) {
        for (const auto &dec : decList_.at(s)) {
          to_be_colored.insert(dec);
        }
      }
    }
  }

  for (const auto &s : importantList_) {
    to_be_colored.erase(s);
  }

  return to_be_colored;
}

template<typename T>
std::set<T> Graph<T>::GetAncestorsOfImportantFast(std::set<T> &id_set) {
  std::set<T> res_set;
  std::list<T> temp_set;
  std::list<T> search_set;

  for (typename std::set<T>::iterator it = importantList_.begin(); it != importantList_.end(); it++) {
    search_set.push_back(*it);
  }

  while (!search_set.empty()) {
    for (auto &i : search_set) {
      for (auto &p : i.GetParents()) {
        auto pos = id_set.find(p);
        if (pos != id_set.end()) {
          temp_set.push_back(*pos);
        }
      }
    }

    search_set.clear();
    search_set = temp_set;
    for (const auto &t : temp_set) {
      res_set.insert(t);
    }
    temp_set.clear();
  }

  return res_set;
}

template<typename T>
std::set<T> Graph<T>::GetImportant() const {
  return importantList_;
}

#endif //PTX_ANALYSER_SRC_DATA_STRUCTURES_GRAPH_H_
