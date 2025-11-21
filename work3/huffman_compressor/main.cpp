#include "FileCompressor.h"
#include <iostream>
#include <string>
#include <iomanip>
#include <filesystem>

using namespace std;

#ifdef _WIN32
    namespace fs = filesystem;
#else
    namespace fs = filesystem;
#endif

void printUsage(const char* programName) {
    cout << "Huffman文件压缩工具\n";
    cout << "用法:\n";
    cout << "  压缩: " << programName << " -c <输入文件> <输出文件>\n";
    cout << "  解压: " << programName << " -d <压缩文件> <输出文件>\n";
    cout << "\n";
    cout << "支持的文件格式:\n";
    cout << "  图像: .jpg, .png, .bmp, .gif, .tiff\n";
    cout << "  文档: .doc, .docx, .pdf, .ppt, .pptx\n";
    cout << "  视频: .mp4, .avi, .mov, .mkv, .wmv\n";
    cout << "  音频: .mp3, .wav, .flac, .aac, .ogg\n";
    cout << "  其他: 任意二进制文件\n";
}

string getFileExtension(const string& filePath) {
    size_t pos = filePath.find_last_of('.');
    if (pos != string::npos) {
        return filePath.substr(pos);
    }
    return "";
}

void printFileInfo(const string& filePath, const string& operation) {
    string ext = getFileExtension(filePath);
    string type = "未知格式";
    
    if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".bmp" || 
        ext == ".gif" || ext == ".tiff" || ext == ".tif") {
        type = "图像文件";
    } else if (ext == ".doc" || ext == ".docx") {
        type = "Word文档";
    } else if (ext == ".pdf") {
        type = "PDF文档";
    } else if (ext == ".ppt" || ext == ".pptx") {
        type = "PPT文档";
    } else if (ext == ".mp4" || ext == ".avi" || ext == ".mov" || 
               ext == ".mkv" || ext == ".wmv" || ext == ".flv") {
        type = "视频文件";
    } else if (ext == ".mp3" || ext == ".wav" || ext == ".flac" || 
               ext == ".aac" || ext == ".ogg" || ext == ".m4a") {
        type = "音频文件";
    } else {
        type = "二进制文件";
    }
    
    cout << "\n[" << operation << "] " << type << ": " << filePath << endl;
}

string formatFileSize(size_t bytes) {
    const char* units[] = {"B", "KB", "MB", "GB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);
    
    while (size >= 1024.0 && unitIndex < 3) {
        size /= 1024.0;
        unitIndex++;
    }
    
    ostringstream oss;
    oss << fixed << setprecision(2) << size << " " << units[unitIndex];
    return oss.str();
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printUsage(argv[0]);
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    // 检查输入文件是否存在
    if (!fs::exists(inputFile)) {
        cerr << "错误：输入文件不存在: " << inputFile << endl;
        return 1;
    }

    FileCompressor compressor;

    if (mode == "-c" || mode == "--compress") {
        // 压缩模式
        printFileInfo(inputFile, "压缩");
        
        // 获取原始文件大小
        size_t originalSize = fs::file_size(inputFile);
        cout << "原始文件大小: " << formatFileSize(originalSize) << endl;
        
        cout << "正在压缩..." << endl;
        
        double compressionRatio = 0.0;
        if (compressor.compressFile(inputFile, outputFile, compressionRatio)) {
            // 获取压缩后文件大小
            size_t compressedSize = fs::file_size(outputFile);
            
            cout << "\n压缩完成！" << endl;
            cout << "压缩后文件大小: " << formatFileSize(compressedSize) << endl;
            cout << "压缩比: " << fixed << setprecision(2) 
                      << (compressionRatio * 100) << "%" << endl;
            cout << "节省空间: " << formatFileSize(originalSize - compressedSize) 
                      << " (" << fixed << setprecision(2) 
                      << ((1.0 - compressionRatio) * 100) << "%)" << endl;
        } else {
            cerr << "压缩失败！" << endl;
            return 1;
        }
        
    } else if (mode == "-d" || mode == "--decompress") {
        // 解压模式
        printFileInfo(inputFile, "解压");
        
        cout << "正在解压..." << endl;
        
        if (compressor.decompressFile(inputFile, outputFile)) {
            size_t decompressedSize = fs::file_size(outputFile);
            cout << "\n解压完成！" << endl;
            cout << "解压后文件大小: " << formatFileSize(decompressedSize) << endl;
        } else {
            cerr << "解压失败！" << endl;
            return 1;
        }
        
    } else {
        cerr << "错误：无效的模式。使用 -c 压缩或 -d 解压" << endl;
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}

