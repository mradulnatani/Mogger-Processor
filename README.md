# Enhanced SimpleOS v2.0

## Overview
Enhanced SimpleOS v2.0 is a lightweight, user-friendly operating system designed for efficient process management and file handling. It offers a simple command-line interface, making it suitable for both novice users and experienced developers.

## Features
- **Process Management**: Create, list, start, and stop processes with ease.
- **File Handling**: Create and list files, managing file sizes effectively.
- **System Information**: Display current system statistics including memory usage and process counts.
- **Colored Output**: Enjoy a visually appealing terminal experience with colored text.
- **Customizable Commands**: Extend functionality with easy-to-use commands.

## Installation
1. Download the source code.
2. Compile the code using a C compiler (e.g., `gcc`):
   ```bash
   gcc -o SimpleOS os.c -std=c99
   ```
3. Run the executable:
   ```bash
   ./SimpleOS
   ```

## Commands
Here’s a list of available commands:
- `create [name] [priority] [memory]`: Create a new process.
- `list`: List all current processes.
- `start [id]`: Start a specified process by ID.
- `stop [id]`: Stop a specified process by ID.
- `createfile [name] [size]`: Create a new file.
- `listfiles`: List all created files.
- `sysinfo`: Display system information.
- `exit`: Exit the OS.

## Contribution
Contributions are welcome! If you would like to suggest features, report bugs, or improve documentation, please feel free to submit a pull request or open an issue on the project repository.

## License
This project is licensed under the MIT License. See the LICENSE file for more details.

## Acknowledgments
- Inspired by traditional operating systems and user feedback.
- Special thanks to the open-source community for their continuous support and resources.

## Contact
For more information, please contact:
- Email: mradulnatani0@gmail.com
- GitHub: mradulnatani

