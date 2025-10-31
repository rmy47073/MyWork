#include <iostream>
#include <string>
#include <stack>
#include <stdexcept>
#include <cstdlib>

using namespace std;


static inline bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}


static inline double dabs(double x) { return x < 0 ? -x : x; }
static inline bool is_integer(double x) {
    // 使用 1e-9 的容差判断浮点是否近似整数
    long long xi = (long long)x;
    return dabs(x - (double)xi) < 1e-9;
}

// 运算符优先级：越高越先计算
int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0; // 括号优先级最低
}

// 执行运算（a: 左操作数, b: 右操作数）
double calculate(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (dabs(b) < 1e-9) throw runtime_error("除数不能为0");
            return a / b;
        default: throw runtime_error("未知运算符");
    }
}

// 检查括号匹配（栈的后入先出特性：左括号入栈，右括号出栈）
bool checkBrackets(const string& expr) {
    stack<char> st;
    for (char c : expr) {
        if (c == '(') {
            st.push(c); // 左括号入栈
        } else if (c == ')') {
            if (st.empty()) return false; // 右括号多，无匹配左括号
            st.pop(); // 右括号匹配最近的左括号（后入先出）
        }
    }
    return st.empty(); // 左括号多则栈非空，即为false
}

// 检查表达式语法错误
bool hasError(const string& expr) {
    if (expr.empty()) {
        cout << "错误：表达式为空";
        return true;
    }
    if (!checkBrackets(expr)) {
        cout << "错误：括号不匹配";
        return true;
    }

    const string ops = "+-*/";
    using idx_t = string::size_type;
    idx_t n = expr.size();

    // 检查首尾字符
    if (ops.find(expr[0]) != string::npos && expr[0] != '-') {
        cout << "错误：开头不能是" << expr[0];
        return true;
    }
    if (ops.find(expr.back()) != string::npos) {
        cout << "错误：结尾不能是" << expr.back();
        return true;
    }

    // 检查相邻字符合法性
    for (idx_t i = 1; i < n-1; i++) {
        char c = expr[i];
        char next = expr[i+1];
        // 运算符后只能接数字或左括号
        if (ops.find(c) != string::npos) {
            if (!is_digit(next) && next != '(') {
                cout << "错误：" << c << "后不能接" << next;
                return true;
            }
        }
        // 左括号后只能接数字或负号
        else if (c == '(') {
            if (!is_digit(next) && next != '-') {
                cout << "错误：(后不能接" << next;
                return true;
            }
        }
        // 右括号后只能接运算符或右括号
        else if (c == ')') {
            if (is_digit(next) || next == '(') {
                cout << "错误：)后不能接" << next;
                return true;
            }
        }
        // 数字/小数点后不能接左括号
        else if ((is_digit(c) ) && next == '(') {
            cout << "错误：" << c << "后不能接(";
            return true;
        }
        // 检查小数点合法性
        if (c == '.') {
            if (i == 0 || !is_digit(expr[i-1]) || !is_digit(next)) {
                cout << "错误：小数点格式不正确";
                return true;
            }
        }
    }
    return false;
}

// 核心计算函数（利用栈的后入先出特性）
double evaluate(const string& expr) {
    stack<double> numStack;  // 存储数字的栈（后入先出）
    stack<char> opStack;     // 存储运算符的栈（后入先出）
    const string ops = "+-*/"; // 运算符集合，用于一元负号判断
    for (string::size_type i = 0; i < expr.size(); i++) {
        char c = expr[i];
        if (c == ' ') continue; // 跳过空格

        // 处理数字（包括多位数和小数）
        if (is_digit(c) || c == '.') {
            string numStr;
            while (i < expr.size() && (is_digit(expr[i]) || expr[i] == '.')) {
                numStr += expr[i++];
            }
            i--; // 回退索引
            numStack.push(stod(numStr)); // 数字入栈（后入先出）
        }
        // 左括号直接入栈
        else if (c == '(') {
            opStack.push(c);
        }
        // 右括号：计算到最近的左括号（栈顶先出）
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                char op = opStack.top(); opStack.pop(); // 运算符出栈（后入先出）
                double b = numStack.top(); numStack.pop(); // 右操作数出栈
                double a = numStack.top(); numStack.pop(); // 左操作数出栈
                numStack.push(calculate(a, b, op)); // 结果入栈
            }
            if (!opStack.empty()) opStack.pop(); // 弹出左括号（如果存在）
        }
        // 处理运算符（考虑优先级和栈的后入先出）
        else {
            // 一元负号处理（补0）
            if (c == '-' && (i == 0 || expr[i-1] == '(' || ops.find(expr[i-1]) != string::npos)) {
                numStack.push(0);
            }
            // 栈顶运算符优先级高则先计算（后入先出）
            while (!opStack.empty() && opStack.top() != '(' && getPriority(opStack.top()) >= getPriority(c)) {
                char op = opStack.top(); opStack.pop();
                double b = numStack.top(); numStack.pop();
                double a = numStack.top(); numStack.pop();
                numStack.push(calculate(a, b, op));
            }
            opStack.push(c); // 当前运算符入栈
        }
    }

    // 处理剩余运算符（栈顶先出）
    while (!opStack.empty()) {
        char op = opStack.top(); opStack.pop();
        double b = numStack.top(); numStack.pop();
        double a = numStack.top(); numStack.pop();
        numStack.push(calculate(a, b, op));
    }

    return numStack.top(); // 栈顶为最终结果
}

int main() {
    string expr;
    char choice;

    cout << "栈实现计算器（支持括号、整数、小数）" << endl;
    cout << "输入表达式（如 (1+2.5)*3 ），输入q退出" << endl;

    do {
        cout << "请输入表达式：";
        getline(cin, expr);

        if (expr == "q" || expr == "Q") break;

        if (hasError(expr)) {
            cout << "，请重新输入！" << endl << endl;
            continue;
        }

        try {
            double result = evaluate(expr);
            // 整数结果格式化输出
            if (is_integer(result)) {
                cout << "结果：" << (int)result << endl;
            } else {
                cout << "结果：" << result << endl;
            }
        } catch (const exception& e) {
            cout << "计算错误：" << e.what() << endl;
        }

        cout << "继续计算？(y/n)：";
        cin >> choice;
        switch (choice) {
            case 'y':
            case 'Y':
                break;
            case 'n':
            case 'N':
                cout << "程序结束！" << endl;
                return 0;
            default:
                cout << "无效输入，程序结束！" << endl;
                return 0;
        }
        cin.ignore(); // 清除缓冲区

    } while (true);
    
    return 0;
}





   


    
   