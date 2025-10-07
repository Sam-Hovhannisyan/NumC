# NumC

NumC is a lightweight C++ library for numerical computing, inspired by NumPy.  
It provides simple array and matrix operations with a familiar interface.

---

## ✨ Features
- Multi-dimensional array support (`Array<T>`)
- Broadcasting functionality
- Element-wise operations
- Easy initialization from lists
- Extensible templates for numeric types

---

## 📦 Installation
Clone the repository:
```bash
git clone https://github.com/Sam-Hovhannisyan/NumC.git
cd NumC
```

## 🛠️ Using 

To create multidimentional array use make_array() function, but for one dimentional cases use a regular constructor

```bash c++
# One dimentional array example
Array<int> arr = {5, 2, 9, 1, 7};
# Multi dimentional array example
Array<int> multi_arr = make_array({{10, 20, 30}, {40, 50, 60}, {40, 50, 60}});

# To pring array's data use print_data()
arr.print_data();
```
