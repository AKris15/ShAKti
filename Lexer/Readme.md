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
[पूर्ण] [संख्या] [=] [10] [|]
[यदि] [(] [संख्या] [>] [5] [)] [{]
[लेख] [(] ["संख्या बड़ी है"] [)] [|]
[}]
```

---

## 🛠️ Running the ShAKti Lexer  

### 📥 Step 1: Organizing Files  
Ensure you have the following files in the same directory:  
- `lexer.c` → The C program implementing the lexer.  
- `input.shakti` → A test file containing ShAKti code to be tokenized.  

Example `input.shakti`:  
```shakti
पूर्ण संख्या = 10|
यदि (संख्या > 5) {
    लेख("संख्या बड़ी है")|
}
```

---

## 🖥️ Windows Instructions  

### 1️⃣ Install a C Compiler (If Not Installed)  
- Install **MinGW** (Minimalist GNU for Windows) and add it to your PATH.  
- Alternatively, install **TDM-GCC** or use **MSYS2**.  

### 2️⃣ Open Command Prompt  
Navigate to the directory where `lexer.c` and `input.shakti` are located:  
```cmd
cd path\to\your\lexer\folder
```

### 3️⃣ Compile the Lexer  
Use GCC to compile the lexer:  
```cmd
gcc lexer.c -o lexer.exe
```
If there are no errors, an executable **`lexer.exe`** will be created.

### 4️⃣ Run the Lexer with Input File  
```cmd
lexer.exe input.shakti
```
The output should display the tokenized form of the input file.

---

## 🐧 Linux Instructions  

### 1️⃣ Install GCC (If Not Installed)  
If you don't have GCC, install it using:  
```bash
sudo apt update && sudo apt install gcc -y   # Ubuntu/Debian
sudo dnf install gcc -y                      # Fedora
sudo pacman -S gcc                           # Arch
```

### 2️⃣ Open Terminal & Navigate to Directory  
```bash
cd /path/to/your/lexer/folder
```

### 3️⃣ Compile the Lexer  
```bash
gcc lexer.c -o lexer
```

### 4️⃣ Run the Lexer with Input File  
```bash
./lexer input.shakti
```

---

## 🔎 Expected Output  
If the lexer correctly tokenizes the input, the output should look like:  
```
[पूर्ण] [संख्या] [=] [10] [|]
[यदि] [(] [संख्या] [>] [5] [)] [{]
[लेख] [(] ["संख्या बड़ी है"] [)] [|]
[}]
```

---

## 🛠️ Debugging Issues  
- If you get `"gcc not recognized"` (Windows), ensure GCC is added to your **PATH**.  
- If you see `"permission denied"` (Linux), make the file executable:  
  ```bash
  chmod +x lexer
  ```
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

