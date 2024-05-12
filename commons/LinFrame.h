class LinFrame {
private:
    bool valid;
    char header;
    char response[];
public:
    LinFrame(char frame[]);

    char getHeader();
    char* getResponse();

    bool isValid();
};