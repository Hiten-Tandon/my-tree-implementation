#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>
namespace fs = std::filesystem;

class FTNode {
private:
  std::string val;
  fs::file_type ft;
  std::unordered_map<std::string, FTNode *> children;

public:
  FTNode(std::string const &val, fs::file_type ft)
      : val(val), ft(ft), children({}){};
  ~FTNode() {
    for (const auto &[_, child] : children) {
      delete child;
    }
  }

  static FTNode *make_file_tree(std::string const &path) {
    FTNode *root = new FTNode{path, fs::file_type::directory};
    std::vector<std::pair<fs::directory_iterator, FTNode *>> frontier{
        {fs::directory_iterator(path), root}};

    while (!frontier.empty()) {
      auto [it, node] = frontier.back();
      frontier.pop_back();

      for (const fs::directory_entry &e : it) {
        std::string path = e.path().string();
        path = path.substr(path.rfind("/") + 1);
        node->children[path] = new FTNode(path, e.symlink_status().type());
        if (e.is_directory()) {
          frontier.emplace_back(fs::directory_entry(e.path()),
                                node->children[path]);
        }
      }
    }
    return root;
  }

  void print(std::vector<bool> &lasts) const {
    int ident = (int)lasts.size() - 1;

    for (int i = 0; i < ident; ++i) {
      std::cout << (lasts[i] ? "  " : "│ ");
    }
    if (ident >= 0) {
      std::cout << (lasts.back() ? "└─" : "├─");
    }

    int child_count = this->children.size();
    std::cout << this->val << std::endl;

    lasts.push_back(false);
    for (const auto [_, child] : this->children) {
      lasts.back() = (child_count <= 1);
      child->print(lasts);
      --child_count;
    }
    lasts.pop_back();
  }
};

int main() {
  FTNode *ft = FTNode::make_file_tree(".");
  std::vector<bool> temp;
  ft->print(temp);
}
/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
