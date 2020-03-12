#include <iostream>
extern int sizeS;
class area {
public:
	char **array;

	area();
	std::string draw();
};

area::area() {
	array = new char *[sizeS];
	for (int i = 0; i < sizeS; i++)
		array[i] = new char[sizeS];
	for (int i = 1; i < sizeS-1; i++) {
		array[i][0] = '-';
		for (int ii = 1; ii < sizeS-1; ii++) {
			array[i][ii] = ' ';
		}
		array[i][sizeS-1] = '-';
	}
	for (int i = 0; i < sizeS; i++) {
		array[0][i] = '-';
		array[sizeS-1][i] = '-';
	}
}

/*char *area::draw() {
	char *buffer = new char[sizeS * sizeS + sizeS];
	for (int i = 0, x = 0; i < sizeS; i++) {
		for (int ii = 0; ii < sizeS; ii++)
			buffer[x + i + ii] = array[i][ii];
		x += sizeS + 1;
		if (i < sizeS - 2)
			buffer[x] = '\n';
		else
			buffer[x] = 0;
	}
	return buffer;
}*/
std::string area::draw() {
	std::string buffer = "";
	for (int i = 0, x = 0; i < sizeS; i++) {
		for (int ii = 0; ii < sizeS; ii++)
			buffer += array[i][ii];
		buffer += '\n';
	}
	return buffer;
}