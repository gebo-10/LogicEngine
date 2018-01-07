#include"component.h"
#include "gebo_file.h"
#include <io.h>
#include<Windows.h>
using namespace std;
class File:public Component
{
public:
    File() {
        register_call<std::string *, std::vector<std::string> *>(Gebo::File::LIST_FILES, std::bind(&File::list_file, this, std::placeholders::_1, std::placeholders::_2));
    }

    ~File()
    {
    }

    void list_file(std::string *path, std::vector<std::string> * files) {
        vector<string> ownname;
        GetAllFiles(*path, *files);
    }

    
    //��ȡ���е��ļ���
    void GetAllFiles(string path, vector<string>& files)
    {

        long   hFile = 0;
        //�ļ���Ϣ  
        struct _finddata_t fileinfo;//�����洢�ļ���Ϣ�Ľṹ��  
        string p;
        if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)  //��һ�β���
        {
            do
            {
                if ((fileinfo.attrib &  _A_SUBDIR))  //������ҵ������ļ���
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)  //�����ļ��в���
                    {
                        files.push_back(p.assign(path).append("\\").append(fileinfo.name));
                        GetAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
                    }
                }
                else //������ҵ��Ĳ������ļ��� 
                {
                    files.push_back(p.assign(fileinfo.name));  //���ļ�·�����棬Ҳ����ֻ�����ļ���:    p.assign(path).append("\\").append(fileinfo.name)
                }

            } while (_findnext(hFile, &fileinfo) == 0);

            _findclose(hFile); //��������
        }

    }

    //��ȡ�ض���ʽ���ļ���
    void GetAllFormatFiles(string path, vector<string>& files, string format)
    {
        //�ļ����  
        long   hFile = 0;
        //�ļ���Ϣ  
        struct _finddata_t fileinfo;
        string p;
        if ((hFile = _findfirst(p.assign(path).append("\\*" + format).c_str(), &fileinfo)) != -1)
        {
            do
            {
                if ((fileinfo.attrib &  _A_SUBDIR))
                {
                    if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
                    {
                        //files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
                        GetAllFormatFiles(p.assign(path).append("\\").append(fileinfo.name), files, format);
                    }
                }
                else
                {
                    files.push_back(p.assign(fileinfo.name));  //���ļ�·�����棬Ҳ����ֻ�����ļ���:    p.assign(path).append("\\").append(fileinfo.name)
                }
            } while (_findnext(hFile, &fileinfo) == 0);

            _findclose(hFile);
        }
    }

};
EXPORT_COMPONENT(File)