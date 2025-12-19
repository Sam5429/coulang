load "libc.so" printf int

fn main() int {
	val i int = 1
	while i < 10 {
		printf("Hello")
		i = (i + 1)
	}
}
