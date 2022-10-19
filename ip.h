#include <string>
#include <vector>
#include <array>

class IP {

public:

    IP(std::vector<std::string> &address);
    IP(std::string &address);

    ~IP() = default;

    static std::vector<std::string> splitBy(std::string &string, char letter);
    static bool isIP(std::string &address);

    std::string asString();
    bool isValid() const;

    bool isFirst(short f)        const;
    bool isTwo(short f, short s) const;
    bool isAny(short a)          const;

    bool operator< (const IP &right);

    short first()   const { return m_ip[0]; }
    short second()  const { return m_ip[1]; }
    short third()   const { return m_ip[2]; }
    short fourth()  const { return m_ip[3]; }

private:

    std::array<int, 4> m_ip;

    void convertToInt(std::vector<std::string> &address);
    std::string convertToStr() const;
    void fromVector(std::vector<std::string> &address);
};