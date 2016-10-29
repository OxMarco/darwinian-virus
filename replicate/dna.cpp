#include <vector>
#include "random.h"

class DNA {
	unsigned int length;
	std::vector<bool> gene;
private:
	void initiate()
	{
		for (register int i = 0; i < length; i++)
			gene[i] = random_int() % 2;
	};
	void mutate();
};