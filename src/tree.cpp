#include <filesystem>
#include <iostream>
#include <vector>
namespace fs = std::filesystem;

class FTNode {
private:
  std::string val;
  fs::file_type ft;
  std::vector<FTNode *> children;

public:
  FTNode(std::string const &val, fs::file_type ft)
      : val(val), ft(ft), children({}){};

  ~FTNode() {
    for (FTNode *const child : children) {
      delete child;
    }
  }

  friend FTNode *make_file_tree(std::string const &path);

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
    for (FTNode *const child : this->children) {
      lasts.back() = (child_count <= 1);
      child->print(lasts);
      --child_count;
    }
    lasts.pop_back();
  }
};

FTNode *make_file_tree(std::string const &path) {
  FTNode *root = new FTNode{path, fs::file_type::directory};
  std::vector<std::pair<fs::directory_iterator, FTNode *>> directory_list{
      {fs::directory_iterator(path), root}};

  while (!directory_list.empty()) {
    auto [it, node] = directory_list.back();
    directory_list.pop_back();

    for (const fs::directory_entry &e : it) {
      std::string path = e.path().string();
      path = path.substr(path.rfind("/") + 1);
      node->children.push_back(new FTNode(path, e.symlink_status().type()));
      if (e.is_directory()) {
        directory_list.emplace_back(fs::directory_entry(e.path()),
                                    node->children.back());
      }
    }
  }
  return root;
}

int main() {
  FTNode *ft = make_file_tree(".");
  std::vector<bool> temp;
  ft->print(temp);
}
