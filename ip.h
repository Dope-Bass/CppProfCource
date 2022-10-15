#include <string>
#include <vector>

class IP {

public:

    IP(std::vector<std::string> &address);
    IP(std::string &address);

    ~IP() = default;

    static std::vector<std::string> splitBy(std::string string, char letter);
    static bool isIP(std::string address);

    std::string asString();
    bool isValid() const;

    bool isFirst(short f)        const;
    bool isTwo(short f, short s) const;
    bool isAny(short a)          const;

    bool operator< (const IP &right);

    short first()   const { return m_first; }
    short second()  const { return m_second; }
    short third()   const { return m_third; }
    short fourth()  const { return m_fourth; }

private:
    short m_first = 0;
    short m_second = 0;
    short m_third = 0;
    short m_fourth = 0;

    std::string m_asString;

    void convertToInt(std::vector<std::string> &address);
    void fromVector(std::vector<std::string> &address);
};