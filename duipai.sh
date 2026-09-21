g++ data.cpp -o data -O2
g++ my.cpp -o my -O2
g++ std.cpp -o std -O2

((cnt=1))

while true
do
	./data > in
	./my < in > out
	./std < in > ans
	if diff out ans; then
		printf "# $((cnt++)) Accepted\n";
	else 
		notify-send "WA"
		break
	fi
done