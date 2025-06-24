cd /usr/local/lib

for lib in avcodec avdevice avfilter avformat avutil swresample swscale; do
    realpath=$(find /usr/local/lib -name "lib${lib}.so.*.*.*" | sort -V | tail -n1)
    if [[ -n "$realpath" ]]; then
        filename=$(basename "$realpath")  # e.g. libavcodec.so.62.4.100
        shortname="lib${lib}.so"
        major=$(echo "$filename" | sed -E 's/.*\.so\.([0-9]+)\..*/\1/')

        echo "🔗 Linking $filename (from $realpath)"
        ln -sf "$realpath" "$filename"
    else
        echo "⚠️ Could not find full version of lib${lib}.so"
    fi
done