#include <string>

typedef struct SMedia
{
    std::string inputUrl;

    SMedia() = default;
    SMedia(const std::string& url) : inputUrl(url){}
};