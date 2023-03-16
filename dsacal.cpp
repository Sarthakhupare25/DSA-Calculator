#include <bits/stdc++.h>
using namespace std;

#define pn pair<node *, node *>

struct node
{
    node *prev;
    int data;
    node *next;
};

bool isValid(string &s);
string postfix(string s);
int precedence(char c);
bool ischar(char c);
node *eval(string s);
pn cal(pn num1, pn num2, char c);
pn insert(node *&a, node *&b, char c);
pn evaladd(pn num1, pn num2);
pn evalsub(pn num1, pn num2);
void traverse(node *head);
int big(node *a, node *b);
int sz(node *a);
pn sub(pn num1, pn num2);
pn evalmul(pn num1, pn num2);
pn evaldiv(pn num1, pn num2);

int main()
{
    string exp;
    cout << "Enter Your Expression : ";
    string final;
    getline(cin, exp);

    if (!isValid(exp))
    {
        cout << "Invalid Expression..";
        exit('0');
    }

    // cout << exp << endl;
    final = postfix(exp);
    // cout << final << endl;

    node *ans = eval(final);
    traverse(ans);

    return 0;
}

// To check wether the given string has valid symbols or not

bool isValid(string &s)
{
    if (!((s[0] >= '0' && s[0] <= '9') || s[0] == '(' || s[0] == '{' || s[0] == '['))
    {
        return false;
    }

    for (auto &i : s)
    {
        if (i == '{' || i == '[')
            i = '(';

        if (i == '}' || i == ']')
            i = ')';

        if (((i >= '0' && i <= '9') || i == '(' || i == ')' || i == '+' || i == '-' || i == '*' || i == '/' || i == ' '))
            continue;
        else
            return false;
    }
    return true;
}

// Precedence .Will be used to get postfix
int precedence(char c)
{
    if (c == '*' || c == '/')
    {
        return 2;
    }

    else if (c == '+' || c == '-')
    {
        return 1;
    }

    else
        return -1;
}

bool ischar(char c)
{
    if ((c <= '9' && c >= '0'))
        return true;

    return false;
}

// To convert given string to its postfix expression
string postfix(string s)
{
    string a = "";
    stack<char> stk;

    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == ' ')
            continue;

        if (ischar(s[i]))
        {
            a += s[i];
        }

        else if (s[i] == '(')
        {
            a += " ";
            stk.push('(');
        }

        else if (s[i] == ')')
        {
            while (stk.top() != '(')
            {
                a = a + " " + stk.top();
                stk.pop();
            }
            stk.pop();
        }

        else
        {
            if (!stk.empty() && s[i] == stk.top() && s[i] == '^')
            {
                stk.push(s[i]);
            }

            else
            {
                while (!stk.empty() && (precedence(stk.top()) >= precedence(s[i])))
                {
                    a = a + " " + stk.top();
                    stk.pop();
                }
                a += " ";
                stk.push(s[i]);
            }
        }
    }

    while (!stk.empty())
    {
        if (stk.top() == '(')
        {
            cout << "Invalid expression" << endl;
            exit('0');
        }
        a = a + " " + stk.top();
        stk.pop();
    }

    return a;
}

// Evaluating postfix expression
node *eval(string s)
{
    stack<pn> stk;
    int i = 0;
    int n = s.length();

    while (i != n)
    {
        if (s[i] == ' ')
        {
            i++;
        }

        else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/' || s[i] == '^')
        {
            if (stk.size() == 1 || stk.size() == 0)
            {
                cout << "Invalid expresssion" << endl;
                exit('0');
            }
            auto num1 = stk.top();
            stk.pop();
            auto num2 = stk.top();
            stk.pop();
            stk.push(cal(num1, num2, s[i]));
            i++;
        }
        else
        {
            pn p;
            node *a = NULL, *b = NULL;
            while (s[i] != ' ')
            {
                p = insert(a, b, s[i]);
                i++;
            }
            stk.push(p);
            i++;
        }
    }
    return stk.top().first;
}

// convering numbers to linked list
pn insert(node *&a, node *&b, char c)
{
    node *p = new node;
    p->data = c - '0';

    if (a == NULL)
    {
        p->prev = NULL;
        p->next = NULL;
        a = p;
        b = p;
        return {a, b};
    }

    b->next = p;
    p->prev = b;
    p->next = NULL;
    b = b->next;
    return {a, b};
}

// assigning operands and operator
pn cal(pn num1, pn num2, char c)
{
    if (c == '+')
    {
        if (num2.first->data == -1 && num1.first->data != -1)
        {
            return evalsub({num2.first->next, num2.second}, num1);
        }

        if (num1.first->data == -1 && num2.first->data != -1)
        {
            return evalsub({num1.first->next, num1.second}, num2);
        }

        if (num2.first->data == -1 && num1.first->data == -1)
        {
            pn ans = evaladd({num2.first->next, num2.second}, {num1.first->next, num1.second});
            node *p = new node;
            p->data = -1;
            p->prev = NULL;
            p->next = ans.first;
            return {p, ans.second};
        }
        return evaladd(num1, num2);
    }

    if (c == '-')
    {
        if (num2.first->data == -1 && num1.first->data != -1)
        {
            pn ans = evaladd({num2.first->next, num2.second}, num1);
            node *p = new node;
            p->data = -1;
            p->prev = NULL;
            p->next = ans.first;
            return {p, ans.second};
        }

        if (num1.first->data == -1 && num2.first->data != -1)
        {
            return evaladd({num1.first->next, num1.second}, num2);
        }

        if (num1.first->data == -1 && num2.first->data == -1)
        {
            return evalsub({num2.first->next, num2.second}, {num1.first->next, num1.second});
        }

        return evalsub(num1, num2);
    }

    if (c == '*')
    {
        if (num1.first->data == -1 && num2.first->data == -1)
        {
            return evalmul({num2.first->next, num2.second}, {num1.first->next, num1.second});
        }

        if (num1.first->data == -1 && num2.first->data != -1)
        {
            pn ans = evalmul(num2, {num1.first->next, num1.second});
            node *p = new node;
            p->data = -1;
            p->prev = NULL;
            p->next = ans.first;
            return {p, ans.second};
        }

        if (num1.first->data != -1 && num2.first->data == -1)
        {
            pn ans = evalmul({num2.first->next, num2.second}, num1);
            node *p = new node;
            p->data = -1;
            p->prev = NULL;
            p->next = ans.first;
            return {p, ans.second};
        }

        return evalmul(num1, num2);
    }

    if (num1.first->data == -1 && num2.first->data == -1)
    {
        return evaldiv({num1.first->next, num1.second}, {num2.first->next, num2.second});
    }

    if (num1.first->data == -1 && num2.first->data != -1)
    {
        pn ans = evaldiv({num1.first->next, num1.second}, num2);
        node *p = new node;
        p->data = -1;
        p->prev = NULL;
        p->next = ans.first;
        return {p, ans.second};
    }

    if (num1.first->data != -1 && num2.first->data == -1)
    {
        pn ans = evaldiv(num1, {num2.first->next, num2.second});
        node *p = new node;
        p->data = -1;
        p->prev = NULL;
        p->next = ans.first;
        return {p, ans.second};
    }

    return evaldiv(num1, num2);
}

// Addition
pn evaladd(pn num1, pn num2)
{
    // traverse(num1.first);
    // cout << endl;
    // traverse(num2.first);
    // cout << endl;
    node *a = num1.second;
    node *b = num2.second;
    int carry = 0;

    node *p = new node;
    node *start = NULL, *end = NULL;
    p->prev = p->next = NULL;
    p->data = (a->data + b->data) % 10;
    carry = (a->data + b->data) / 10;
    start = end = p;
    a = a->prev;
    b = b->prev;

    while (a && b)
    {
        node *ptr = new node;
        ptr->prev = ptr->next = NULL;
        ptr->data = (a->data + b->data + carry) % 10;
        carry = (a->data + b->data + carry) / 10;
        if (start == end)
        {
            ptr->next = end;
            end->prev = ptr;
            start = ptr;
        }
        else
        {
            ptr->next = start;
            start->prev = ptr;
            start = ptr;
        }
        a = a->prev;
        b = b->prev;
    }

    if (a)
    {
        start->prev = a;
        b = a;
    }
    else
    {
        start->prev = b;
        a = b;
    }

    if (a)
    {
        a->next = start;
    }

    while (carry && a)
    {
        int temp = a->data + carry;
        a->data = temp % 10;
        carry = temp / 10;
        start = a;
        a = a->prev;
    }

    while (a)
    {
        start = a;
        a = a->prev;
    }

    if (carry)
    {
        node *ptr = new node;
        ptr->data = 1;
        ptr->prev = NULL;
        ptr->next = start;
        start->prev = ptr;
        start = ptr;
    }

    return {start, end};
}

void traverse(node *head)
{
    if (head->data == -1)
    {
        cout << "-";
        head = head->next;
    }

    while (head)
    {
        cout << head->data;
        head = head->next;
    }
    cout << endl;
}

// To calculate length of the linked list
int sz(node *a)
{
    int count = 0;
    while (a)
    {
        count++;
        a = a->next;
    }
    return count;
}

// To get which number is bigger
int big(node *a, node *b)
{
    int num1 = sz(a);
    int num2 = sz(b);
    if (num1 > num2)
    {
        return 1;
    }

    else if (num1 < num2)
    {
        return 2;
    }

    else
    {
        while (a && b)
        {
            if (a->data > b->data)
            {
                return 1;
            }

            else if (a->data < b->data)
            {
                return 2;
            }

            a = a->next;
            b = b->next;
        }
    }
    return 0;
}

// function for substraction
pn evalsub(pn num1, pn num2)
{
    // traverse(num1.first);
    // cout << endl;
    // traverse(num2.first);
    // cout << endl;
    int a = big(num1.first, num2.first);

    if (a == 0)
    {
        node *a = new node;
        a->data = 0;
        a->next = NULL;
        a->prev = NULL;
        return {a, a};
    }

    else if (a == 1)
    {
        pn ans = sub(num1, num2);
        node *p = new node;
        p->data = -1;
        p->prev = NULL;
        p->next = ans.first;
        return {p, ans.second};
    }

    pn ans = sub(num2, num1);
    return ans;
}

// subtraction
pn sub(pn num1, pn num2)
{
    node *a = num1.second;
    node *b = num2.second;
    node *head = NULL;
    node *tail = NULL;

    int carry = 0;
    while (b)
    {
        if (head == NULL)
        {
            node *p = new node;
            p->next = p->prev = NULL;
            if (a->data < b->data)
            {
                carry = 1;
                p->data = 10 + a->data - b->data;
            }
            else
            {
                carry = 0;
                p->data = a->data - b->data;
            }
            head = tail = p;
        }

        else
        {
            node *p = new node;
            if (a->data - carry < b->data)
            {
                p->data = 10 + a->data - b->data - carry;
                carry = 1;
            }
            else
            {
                p->data = a->data - b->data - carry;
                carry = 0;
            }
            p->next = head;
            p->prev = NULL;
            head->prev = p;
            head = p;
        }

        a = a->prev;
        b = b->prev;
    }

    while (a)
    {
        node *p = new node;
        if (carry)
        {
            if (a->data == 0)
            {
                carry = 1;
                p->data = 9;
            }
            else
            {
                carry = 0;
                p->data = a->data - 1;
            }
        }
        else
        {
            p->data = a->data;
        }
        p->next = head;
        p->prev = NULL;
        head->prev = p;
        head = p;
        a = a->prev;
    }

    // removing zeros at the front
    while (head->data == 0)
    {
        node *p = head;
        head = head->next;
        delete (p);
    }
    head->prev = NULL;
    return {head, tail};
}

// function for multiplication
pn evalmul(pn num1, pn num2)
{
    if (num1.first->data == 0 || num2.first->data == 0)
    {
        node *p = new node;
        p->data = 0;
        p->prev = p->next = NULL;
        return {p, p};
    }

    node *head = new node;
    head->data = 0;
    head->prev = NULL;
    head->next = NULL;
    node *tail = head;

    node *a = num1.second;
    while (a)
    {
        node *b = num2.second;
        node *h = NULL;
        node *t = NULL;
        int carry = 0;
        while (b)
        {
            node *p = new node;
            int temp = a->data * b->data + carry;
            p->data = temp % 10;
            carry = temp / 10;
            if (h == NULL)
            {
                p->next = NULL;
                p->prev = NULL;
                h = p;
                t = p;
            }
            else
            {
                p->next = h;
                h->prev = p;
                p->prev = NULL;
                h = p;
            }
            b = b->prev;
        }

        if (carry)
        {
            node *p = new node;
            p->data = carry;
            p->next = h;
            p->prev = NULL;
            h->prev = p;
            h = p;
        }

        pn ans = evaladd({head, tail}, {h, t});
        head = ans.first;
        tail = ans.second;
        node *p = new node;
        p->data = 0;
        num2.second->next = p;
        p->prev = num2.second;
        p->next = NULL;
        num2.second = p;

        a = a->prev;
    }
    return {head, tail};
}

pn evaldiv(pn num1, pn num2)
{
    node *a = new node;
    a->data = 0;
    a->prev = NULL;
    a->next = NULL;
    pn ans = {a, a};

    node *ptr = new node;
    ptr->data = 1;
    ptr->prev = NULL;
    ptr->next = NULL;
    pn one = {ptr, ptr};

    pn temp = num1;

    while (big(num2.first, num1.first) != 2)
    {
        ans = evaladd(ans, one);
        num1 = evaladd(num1, temp);
    }

    return ans;
}