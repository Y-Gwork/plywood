/*------------------------------------
  ///\  Plywood C++ Framework
  \\\/  https://plywood.arc80.com/
------------------------------------*/
#include <ply-runtime/Base.h>

using namespace ply;

template <typename T>
void testParse(StringWriter* outs, StringView type, StringView str, u32 radix) {
    StringViewReader sr{str};
    T result = sr.parse<T>(fmt::Radix{radix});
    String backToString = String::from(fmt::WithRadix{result, radix});
    outs->format("Parsing \"{}\" as {}, radix {} -> \"{}\", {}\n", str, type, radix, backToString,
                 sr.anyParseError() ? "failed" : "success");
}

StringView strList[] = {
    "123",
    "24000000",
    "7fffffff",
    "80000000",
    "ffffffff",
    "100000000",
    "100000001",
    "214748364",
    "214748365",
    "240000001",
    "429496729",
    "429496730",
    "2400000000000000",
    "7fffffffffffffff",
    "8000000000000000",
    "ffffffffffffffff",
    "10000000000000000",
    "10000000000000001",
    "24000000000000001",
    "922337203685477580",
    "922337203685477581",
    "1844674407370955161",
    "1844674407370955162",
};

int main() {
    StringWriter outs;
    for (u32 i = 0; i < PLY_STATIC_ARRAY_SIZE(strList); i++) {
        for (u32 s = 0; s < 2; s++) {
            String str = strList[i];
            if (s == 1) {
                str = StringView{"-"} + str;
            }
            for (u32 radix : {10, 16}) {
                testParse<u32>(&outs, "u32", str, radix);
                testParse<s32>(&outs, "s32", str, radix);
                testParse<u64>(&outs, "u64", str, radix);
                testParse<s64>(&outs, "s64", str, radix);
            }
        }
    }
    FileSystem::native()->makeDirsAndSaveTextIfDifferent(
        NativePath::join(PLY_WORKSPACE_FOLDER,
                         "repos/plywood/src/apps/StringReaderTest/result.txt"),
        outs.moveToString(), TextFormat::platformPreference());
    return 0;
}
