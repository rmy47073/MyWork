#include <windows.h>
#include <string>
#include <stack>
#include <stdexcept>
#include <iostream>

using namespace std;

// 全局变量
HWND hDisplay;           // 显示屏幕
HWND hButtons[20];       // 按钮数组
string currentInput = ""; // 当前输入
string lastResult = "";   // 上次结果
bool isNewInput = true;   // 是否为新输入

// 原有计算逻辑函数
static inline bool is_digit(char ch) {
    return ch >= '0' && ch <= '9';
}

static inline double dabs(double x) { return x < 0 ? -x : x; }
static inline bool is_integer(double x) {
    long long xi = (long long)x;
    return dabs(x - (double)xi) < 1e-9;
}

int getPriority(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

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

bool checkBrackets(const string& expr) {
    stack<char> st;
    for (char c : expr) {
        if (c == '(') {
            st.push(c);
        } else if (c == ')') {
            if (st.empty()) return false;
            st.pop();
        }
    }
    return st.empty();
}

bool hasError(const string& expr) {
    if (expr.empty()) return true;
    if (!checkBrackets(expr)) return true;

    const string ops = "+-*/";
    using idx_t = string::size_type;
    idx_t n = expr.size();

    if (ops.find(expr[0]) != string::npos && expr[0] != '-') return true;
    if (ops.find(expr.back()) != string::npos) return true;

    for (idx_t i = 1; i < n-1; i++) {
        char c = expr[i];
        char next = expr[i+1];
        if (ops.find(c) != string::npos) {
            if (!is_digit(next) && next != '(') return true;
        }
        else if (c == '(') {
            if (!is_digit(next) && next != '-') return true;
        }
        else if (c == ')') {
            if (is_digit(next) || next == '(') return true;
        }
        else if ((is_digit(c)) && next == '(') return true;
        if (c == '.') {
            if (i == 0 || !is_digit(expr[i-1]) || !is_digit(next)) return true;
        }
    }
    return false;
}

double evaluate(const string& expr) {
    stack<double> numStack;
    stack<char> opStack;
    const string ops = "+-*/";
    
    for (string::size_type i = 0; i < expr.size(); i++) {
        char c = expr[i];
        if (c == ' ') continue;

        if (is_digit(c) || c == '.') {
            string numStr;
            while (i < expr.size() && (is_digit(expr[i]) || expr[i] == '.')) {
                numStr += expr[i++];
            }
            i--;
            numStack.push(stod(numStr));
        }
        else if (c == '(') {
            opStack.push(c);
        }
        else if (c == ')') {
            while (!opStack.empty() && opStack.top() != '(') {
                char op = opStack.top(); opStack.pop();
                double b = numStack.top(); numStack.pop();
                double a = numStack.top(); numStack.pop();
                numStack.push(calculate(a, b, op));
            }
            if (!opStack.empty()) opStack.pop();
        }
        else {
            if (c == '-' && (i == 0 || expr[i-1] == '(' || ops.find(expr[i-1]) != string::npos)) {
                numStack.push(0);
            }
            while (!opStack.empty() && opStack.top() != '(' && getPriority(opStack.top()) >= getPriority(c)) {
                char op = opStack.top(); opStack.pop();
                double b = numStack.top(); numStack.pop();
                double a = numStack.top(); numStack.pop();
                numStack.push(calculate(a, b, op));
            }
            opStack.push(c);
        }
    }

    while (!opStack.empty()) {
        char op = opStack.top(); opStack.pop();
        double b = numStack.top(); numStack.pop();
        double a = numStack.top(); numStack.pop();
        numStack.push(calculate(a, b, op));
    }

    return numStack.top();
}

// GUI相关函数
void updateDisplay() {
    SetWindowTextA(hDisplay, currentInput.c_str());
}

void clearInput() {
    currentInput = "";
    isNewInput = true;
    updateDisplay();
}

void appendToInput(const string& str) {
    if (isNewInput) {
        currentInput = str;
        isNewInput = false;
    } else {
        currentInput += str;
    }
    updateDisplay();
}

void calculateResult() {
    if (currentInput.empty()) return;
    
    try {
        if (hasError(currentInput)) {
            currentInput = "错误：表达式格式不正确";
            isNewInput = true;
            updateDisplay();
            return;
        }
        
        double result = evaluate(currentInput);
        if (is_integer(result)) {
            currentInput = to_string((int)result);
        } else {
            currentInput = to_string(result);
        }
        lastResult = currentInput;
        isNewInput = true;
        updateDisplay();
    } catch (const exception& e) {
        currentInput = "错误：" + string(e.what());
        isNewInput = true;
        updateDisplay();
    }
}

void deleteLastChar() {
    if (!currentInput.empty()) {
        currentInput.pop_back();
        updateDisplay();
    }
}

// 按钮点击处理
void onButtonClick(int buttonId) {
    switch (buttonId) {
        case 0: clearInput(); break;        // C
        case 1: deleteLastChar(); break;    // ?
        case 2: appendToInput("0"); break;  // 0
        case 3: appendToInput("("); break;  // (
        case 4: appendToInput(")"); break;  // )
        case 5: appendToInput("1"); break;  // 1
        case 6: appendToInput("2"); break;  // 2
        case 7: appendToInput("3"); break;  // 3
        case 8: appendToInput("+"); break;  // +
        case 9: appendToInput("-"); break;  // -
        case 10: appendToInput("4"); break; // 4
        case 11: appendToInput("5"); break; // 5
        case 12: appendToInput("6"); break; // 6
        case 13: appendToInput("*"); break; // *
        case 14: appendToInput("/"); break; // /
        case 15: appendToInput("7"); break; // 7
        case 16: appendToInput("8"); break; // 8
        case 17: appendToInput("9"); break; // 9
        case 18: appendToInput("."); break; // .
        case 19: calculateResult(); break;  // =
    }
}

// 窗口过程
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            // 创建显示屏幕
            hDisplay = CreateWindowA(
                "EDIT", "",
                WS_VISIBLE | WS_CHILD | ES_RIGHT | ES_READONLY,
                10, 10, 280, 40,
                hwnd, NULL, NULL, NULL
            );
            
            // 设置显示屏幕字体
            HFONT hFont = CreateFontA(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Consolas");
            SendMessage(hDisplay, WM_SETFONT, (WPARAM)hFont, TRUE);
            
            // 按钮标签和位置
            const char* buttonLabels[] = {
                "C", "Del", "0", "(", ")",  // 第一行：清除，删除，0，（，）
                "1", "2", "3", "+", "-",  // 第二行：1，2，3，+，-
                "4", "5", "6", "*", "/",  // 第三行：4，5，6，*，/
                "7", "8", "9", ".", "="   // 第四行：7，8，9，.，=
            };
            
            int buttonPositions[][4] = {
                {10, 60, 50, 40},   // C
                {70, 60, 50, 40},   // ?
                {130, 60, 50, 40},  // 0
                {190, 60, 50, 40},  // (
                {250, 60, 50, 40},  // )
                {10, 110, 50, 40},  // 1
                {70, 110, 50, 40},  // 2
                {130, 110, 50, 40}, // 3
                {190, 110, 50, 40}, // +
                {250, 110, 50, 40}, // -
                {10, 160, 50, 40},  // 4
                {70, 160, 50, 40},  // 5
                {130, 160, 50, 40}, // 6
                {190, 160, 50, 40}, // *
                {250, 160, 50, 40}, // /
                {10, 210, 50, 40},  // 7
                {70, 210, 50, 40},  // 8
                {130, 210, 50, 40}, // 9
                {190, 210, 50, 40}, // .
                {250, 210, 50, 40}  // =
            };
            
            // 创建按钮
            for (int i = 0; i < 20; i++) {
                hButtons[i] = CreateWindowA(
                    "BUTTON", buttonLabels[i],
                    WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                    buttonPositions[i][0], buttonPositions[i][1],
                    buttonPositions[i][2], buttonPositions[i][3],
                    hwnd, (HMENU)(INT_PTR)(i + 100), NULL, NULL
                );
                
                // 设置按钮字体
                SendMessage(hButtons[i], WM_SETFONT, (WPARAM)hFont, TRUE);
            }
            
            // 初始化显示
            updateDisplay();
            break;
        }
        
        case WM_COMMAND: {
            int buttonId = LOWORD(wParam) - 100;
            if (buttonId >= 0 && buttonId < 20) {
                onButtonClick(buttonId);
            }
            break;
        }
        
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // 注册窗口类
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "CalculatorWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    
    RegisterClassA(&wc);
    
    // 创建窗口
    HWND hwnd = CreateWindowA(
        "CalculatorWindow",
        "计算器",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT,
        320, 350,
        NULL, NULL, hInstance, NULL
    );
    
    if (hwnd == NULL) {
        return 0;
    }
    
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // 消息循环
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}
