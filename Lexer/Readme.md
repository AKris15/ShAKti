# ShAKti Lexer  
      ~ Sanskrit-Inspired Lexical Analyzer

## 🔥 Introduction  
The **ShAKti Lexer** is a fundamental component of the ShAKti programming language, responsible for breaking down the Sanskrit-inspired code into meaningful tokens. Developed in **C**, it ensures an efficient and accurate interpretation of the ShAKti syntax.

---

## 🚀 Why ShAKti Lexer?  
- ⚡ **Optimized for Performance** – Designed to tokenize code quickly and efficiently.  
- 🧠 **Sanskrit-Based Tokenization** – Recognizes and processes Sanskrit keywords seamlessly.  
- 🔄 **Error Handling** – Provides meaningful error messages for syntax clarity.  
- 🏗️ **Custom Implementation** – Built from scratch to provide a unique language parsing experience.  

---

## 🔧 Features  
✅ **Recognizes Sanskrit Keywords** – Identifies ShAKti-specific reserved words.  
✅ **Efficient Tokenization** – Optimized for speed and memory management.  
✅ **Handles Comments & Whitespace** – Ensures smooth processing.  

---

## 📜 How It Works  
1. Reads the source code as input.  
2. Identifies tokens such as keywords, identifiers, literals, and operators.  
3. Ignores whitespace and comments.  
4. Generates a stream of tokens to be processed by the **ShAKti Parser**.  

---

## 💻 Example Code (Lexer in Action)  
```shakti
पूर्ण संख्या = 10|
यदि (संख्या > 5) {
    लेख("संख्या बड़ी है")|
}
```
The lexer breaks this into tokens:  
```
Current locale: en_US.UTF-8


Lexical Analysis:
Keyword: पूर्ण
Variable: संख्या
Operator: =
Number: 10
End of Line: |
Keyword: यदि
Special Symbol: (
Variable: संख्या
Operator: >
Number: 5
Special Symbol: )
Special Symbol: {
Keyword: लेख
Special Symbol: (
String: "संख्या बड़ी है"
Special Symbol: )
End of Line: |
Special Symbol: }
End of Input: EOF

```

---


## 🛠️ Running the ShAKti Lexer  

### 📁 Preparing Your Environment  
Ensure the following files are in the same directory:  

- **Source Files:** `file_io.c`, `file_io.h`, `Lexer.c`, `Lexer.h`, `main.c`, `Makefile`, `Tokens.c`, `Tokens.h`, `utils.c`, `utils.h`  
- **Input File:** `Short_Input.txt` (Contains the ShAKti code to be tokenized)  

---

### 🚀 Compilation & Execution  

#### 🔹 **For Linux/macOS Users**  

1️⃣ **Install GCC (If Not Installed)**  
```bash
sudo apt update && sudo apt install gcc -y   # Ubuntu/Debian  
sudo dnf install gcc -y                      # Fedora  
sudo pacman -S gcc                           # Arch  
```

2️⃣ **Navigate to the Lexer Directory**  
```bash
cd /path/to/ShAKti/lexer
```

3️⃣ **Compile the Lexer**  
```bash
make
```
This compiles all necessary files and generates the `ShAKti_Lexer` executable.

4️⃣ **Run the Lexer with an Input File**  
```bash
./ShAKti_Lexer Short_Input.txt
```

---

#### 🔹 **For Windows Users**  

1️⃣ **Install a C Compiler (If Not Installed)**  
- Install **MinGW-w64** and ensure `gcc.exe` is added to your system's **PATH**.  
- Alternatively, install **MSYS2** and set up `mingw-w64`.  

2️⃣ **Open Command Prompt and Navigate to the Lexer Directory**  
```cmd
cd C:\path\to\shakti\lexer
```

3️⃣ **Compile the Lexer**  
```cmd
mingw32-make
```
This will generate an executable file: `ShAKti_Lexer.exe`.

4️⃣ **Run the Lexer with an Input File**  
```cmd
ShAKti_Lexer.exe Short_Input.txt
```

---

### 🎯 Expected Output  
If the lexer functions correctly, the output should resemble:  
```
Current locale: en_US.UTF-8


Lexical Analysis:
Keyword: पूर्ण
Variable: संख्या
Operator: =
Number: 10
End of Line: |
Keyword: यदि
Special Symbol: (
Variable: संख्या
Operator: >
Number: 5
Special Symbol: )
Special Symbol: {
Keyword: लेख
Special Symbol: (
String: "संख्या बड़ी है"
Special Symbol: )
End of Line: |
Special Symbol: }
End of Input: EOF

```

---

### 🔎 Debugging Issues  

- **"gcc not recognized" (Windows)** → Ensure MinGW is installed and added to your system’s **PATH**.  
- **"Permission denied" (Linux/macOS)** → Grant executable permission:  
  ```bash
  chmod +x ShAKti_Lexer
  ```
- **"No such file or directory"** → Ensure you're running the command in the correct directory.  

---

🚀 **You're now ready to tokenize ShAKti code!** 🎉

---

## 🤝 Contributing  
We welcome contributions! Feel free to improve the efficiency and functionality of the ShAKti Lexer.

---

## 📜 License  
ShAKti Lexer is **open-source** under the **GPLv2 License**.

---

## 🌐 Community & Support  
📩 **Contact the Devs**: AK & Dishant at  
✉️ [herogupta626@protonmail.com]  
✉️ [dishantsinghrathore786@gmail.com]  

🚀 Let’s build a powerful lexical analyzer together!

