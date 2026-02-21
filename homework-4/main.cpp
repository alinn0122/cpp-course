#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <sstream>
#include <algorithm>

struct IpAddress {
    std::array<int, 4> bytes{};

    static IpAddress parse(const std::string& text) {
        IpAddress ip;
        std::stringstream ss(text);
        std::string part;
        int i = 0;

        while (std::getline(ss, part, '.'))
            ip.bytes[i++] = std::stoi(part);

        return ip;
    }

    bool operator<(const IpAddress& other) const {
        return bytes < other.bytes;
    }
};

std::ostream& operator<<(std::ostream& os, const IpAddress& ip) {
    os << ip.bytes[0] << '.'
        << ip.bytes[1] << '.'
        << ip.bytes[2] << '.'
        << ip.bytes[3];
    return os;
}


/*
* return:
    0 - успешное выполнение программы
    1 - ошибка чтения файла
*/
int main() {
    std::ifstream file("ip_filter.tsv");
    if (!file) {
        std::cerr << "Cannot open ip_filter.tsv\n";
        return -1;
    }

    std::vector<IpAddress> ips;
    std::string line;

    while (std::getline(file, line)) {
        auto tab_pos = line.find('\t');
        if (tab_pos == std::string::npos)
            continue;

        std::string ip_str = line.substr(0, tab_pos);
        ips.push_back(IpAddress::parse(ip_str));
    }

    std::sort(ips.rbegin(), ips.rend());

    for (const auto& ip : ips)  // Вывод всех ip адресов (фильтр 1)
        std::cout << ip << '\n';

    for (const auto& ip : ips)  // Вывод ip адресов с 1 в первом байте (1.х.х.х)
        if (ip.bytes[0] == 1)
            std::cout << ip << '\n';

    for (const auto& ip : ips)  // Вывод ip адресов с 46 в первом байте и 70 во втором байте (46.70.х.х)
        if (ip.bytes[0] == 46 && ip.bytes[1] == 70)
            std::cout << ip << '\n';

    for (const auto& ip : ips)  // Вывод ip адресов, содержащих 46
        if (std::any_of(ip.bytes.begin(), ip.bytes.end(),
            [](int b) { return b == 46; }))
            std::cout << ip << '\n';

    return 0;
}