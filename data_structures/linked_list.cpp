/**
 * @file linked_list.cpp
 * @brief 单向链表算法的实现。
 * @details
 * 链表是一种数据结构，用于存储一系列的值，支持添加、删除和显示操作。
 * ### 算法细节
 * 添加值：从头节点开始遍历（顺着指针走），当某个节点的指针指向 nullptr 时，
 * 认为它是最后一个节点，让它的指针指向新创建的值。
 * 删除值：同样遍历链表。找到包含目标值的节点后，让它的前驱节点指向它的后继节点，
 * 然后将当前节点归还给堆内存（在智能指针的帮助下自动完成）。
 */
#include <iostream>  // 输入输出流标准库
#include <memory>    // 现代C++核心：智能指针库 (std::shared_ptr, std::make_shared)
#include <string>    // 字符串处理库

/**
 * @namespace data_structures
 * @brief 数据结构相关的命名空间
 */
namespace data_structures {

/**
 * @namespace linked_list
 * @brief 单向链表算法的相关函数/类
 */
namespace linked_list {

/**
 * 判断传入的字符串是否只由数字组成。
 * @param s 需要检查的字符串
 * @returns 如果全是数字返回 true
 * @returns 如果发现任何非数字字符返回 false
 */
bool isDigit(const std::string& s) {
    // 基于范围的 for 循环 (C++11 特性)，遍历字符串中的每个字符
    for (char i : s) {
        if (!isdigit(i)) {  // 如果发现任何一个字符不是数字
            return false;   // 直接返回 false
        }
    }
    return true;  // 所有字符都是数字，返回 true
}

/**
 * 链表节点类（Link），包含一个值和一个指向下一个节点的智能指针。
 */
class link {
 private:
    int pvalue;                   ///< 当前节点存储的整数值
    std::shared_ptr<link> psucc;  ///< 核心！使用 std::shared_ptr 代替裸指针，实现自动内存管理

 public:
    /**
     * 返回当前节点存储的整数值。
     */
    int val() { return pvalue; }

    /**
     * 返回指向下一个节点的智能指针的引用（允许修改指向）。
     */
    std::shared_ptr<link>& succ() { return psucc; }

    /**
     * 构造函数：创建一个带有指定值的节点，且最初指向空（nullptr）。
     * explicit 关键字防止隐式类型转换（例如避免 10 被悄悄转换成节点对象）。
     * @param value 节点存储的整数值，默认为 0
     */
    explicit link(int value = 0) : pvalue(value), psucc(nullptr) {}
};

/**
 * 链表类（list），管理一系列节点。
 */
class list {
 private:
    std::shared_ptr<link> first;  ///< 哨兵节点（Dummy Head），它的数据无意义，但它的 succ 指向链表的第一个真实节点。好处是操作头节点时无需特殊处理！
    std::shared_ptr<link> last;   ///< 链表的最后一个真实节点，方便实现 O(1) 复杂度的尾插

 public:
    /**
     * 构造函数：初始化 first 和 last 指针。
     */
    list() {
        // 初始化哨兵节点（它自身的数据是默认的0，不参与实际数据存储）
        first = std::make_shared<link>();
        // 此时链表为空，last 指向空
        last = nullptr;
    }

    // 声明成员函数
    bool isEmpty();                  // 判断链表是否为空
    void push_back(int new_elem);    // 尾部插入
    void push_front(int new_elem);   // 头部插入
    void erase(int old_elem);        // 删除指定元素
    void display();                  // 打印链表
    std::shared_ptr<link> search(int find_elem);  // 搜索元素
    void reverse();                  // 反转链表（当前代码中未实现）
};

/**
 * 检查链表是否为空。
 */
bool list::isEmpty() {
    // 因为 last 只有在链表有真实节点时才不为空，所以检查 last 即可
    if (last == nullptr) {
        return true;
    } else {
        return false;
    }
}

/**
 * 在链表尾部添加一个新元素。
 * @param new_elem 要添加的整数
 */
void list::push_back(int new_elem) {
    if (isEmpty()) {
        // 如果链表为空：让哨兵节点指向新节点
        first->succ() = std::make_shared<link>(new_elem);
        // 更新尾部指针
        last = first->succ();
    } else {
        // 如果链表不为空：让当前的尾节点指向新节点
        last->succ() = std::make_shared<link>(new_elem);
        // 更新尾指针，将其移动到新的尾节点
        last = last->succ();
    }
}

/**
 * 在链表头部添加一个新元素。
 * @param new_elem 要添加的整数
 */
void list::push_front(int new_elem) {
    if (isEmpty()) {
        // 与 push_back 同理，链表为空时，头插和尾插是一样的
        first->succ() = std::make_shared<link>(new_elem);
        last = first->succ();
    } else {
        // 核心：利用哨兵节点，头部插入不需要修改链表头指针（没有二级指针的烦恼）
        std::shared_ptr<link> t = std::make_shared<link>(new_elem); // 创建新节点
        t->succ() = first->succ(); // 新节点的后继指向原来的首元节点
        first->succ() = t;         // 哨兵节点指向新的首元节点
    }
}

/**
 * 从链表中删除指定元素。
 * @param old_elem 要被删除的整数
 */
void list::erase(int old_elem) {
    if (isEmpty()) {
        std::cout << "List is Empty!";
        return;
    }
    std::shared_ptr<link> t = first; // 从哨兵节点开始遍历
    std::shared_ptr<link> to_be_removed = nullptr; // 临时保存待删除的节点

    // 寻找待删除节点的【前驱节点】（目标是让 t 停在待删除节点的前一个位置）
    // 条件：不是最后一个节点，且下一个节点的值不等于目标值
    while (t != last && t->succ()->val() != old_elem) {
        t = t->succ();
    }

    // 如果遍历到了最后一个节点都没找到目标值
    if (t == last) {
        std::cout << "Element not found\n";
        return;
    }

    // 找到了目标值，t 是它的前驱
    to_be_removed = t->succ();           // 记录要删除的节点
    t->succ() = t->succ()->succ();       // 前驱节点的后继，直接跳过目标节点，指向目标节点的后继
    to_be_removed.reset();               // 核心：手动触发智能指针重置。引用计数减一，如果没有其他指针指向它，该节点内存会被自动释放，无需 delete！

    // 如果删除的是尾部节点，必须更新 last 指针
    if (t->succ() == nullptr) {
        last = t;
    }
    // 如果删完发现链表变空了，把 last 置空
    if (first == last) {
        last = nullptr;
    }
}

/**
 * 打印链表中的所有元素。
 */
void list::display() {
    if (isEmpty()) {
        std::cout << "List is Empty!";
        return;
    }
    std::shared_ptr<link> t = first; // 从哨兵节点开始
    // 遍历链表，只要后面还有节点
    while (t->succ() != nullptr) {
        std::cout << t->succ()->val() << "\t"; // 打印后继节点的值（跳过哨兵节点的0）
        t = t->succ(); // 指针后移
    }
}

/**
 * 在链表中查找指定元素。
 * @param find_elem 要查找的整数
 * @returns 找到则返回该节点的智能指针，没找到返回 nullptr
 */
std::shared_ptr<link> list::search(int find_elem) {
    if (isEmpty()) {
        std::cout << "List is Empty!";
        return nullptr;
    }
    std::shared_ptr<link> t = first;
    // 遍历查找，逻辑与 erase 中的寻找一模一样
    while (t != last && t->succ()->val() != find_elem) {
        t = t->succ();
    }
    if (t == last) {
        std::cout << "Element not found\n";
        return nullptr;
    }
    std::cout << "Element was found\n";
    return t->succ(); // 返回目标节点的智能指针
}
}  // namespace linked_list
}  // namespace data_structures

/**
 * 主函数：
 * 提供菜单供用户添加、删除、搜索和显示链表中的值。
 * @returns 程序退出时返回 0
 */
int main() {
    // 实例化链表对象（注意：要使用完整的命名空间路径）
    data_structures::linked_list::list l;
    int choice = 0; // 用户菜单选项
    int x = 0;      // 存储转换后的数字
    std::string s;  // 存储用户的字符串输入（用于输入校验）

    do {
        // 打印菜单
        std::cout << "\n1. Insert";
        std::cout << "\n2. Delete";
        std::cout << "\n3. Search";
        std::cout << "\n4. Print";
        std::cout << "\n0. Exit";
        std::cout << "\n\nEnter you choice : ";
        std::cin >> choice;

        switch (choice) {
            case 0:
                std::cout << "\nQuitting the program...\n";
                break;
            case 1:
                std::cout << "\nEnter the element to be inserted : ";
                std::cin >> s; // 先作为字符串读入，防止用户输入字母导致程序崩溃

                // 使用 isDigit 校验输入，只有纯数字才能转换
                if (data_structures::linked_list::isDigit(s)) {
                    x = std::stoi(s); // 字符串转整数
                    l.push_back(x);   // 尾部插入
                } else {
                    std::cout << "Wrong Input!\n";
                }
                break;
            case 2:
                std::cout << "\nEnter the element to be removed : ";
                std::cin >> s;
                if (data_structures::linked_list::isDigit(s)) {
                    x = std::stoi(s);
                    l.erase(x); // 删除
                } else {
                    std::cout << "Wrong Input!\n";
                }
                break;
            case 3:
                std::cout << "\nEnter the element to be searched : ";
                std::cin >> s;
                if (data_structures::linked_list::isDigit(s)) {
                    x = std::stoi(s);
                    // 进行搜索
                    std::shared_ptr<data_structures::linked_list::link> found = l.search(x);
                } else {
                    std::cout << "Wrong Input!\n";
                }
                break;
            case 4:
                l.display(); // 打印链表
                std::cout << "\n";
                break;
            default:
                std::cout << "Invalid Input\n" << std::endl;
                break;
        }
    } while (choice != 0); // 只要不选0，就一直循环

    return 0;
}