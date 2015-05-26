#pragma once
class PontoNaTela
{
public:
	int x;
	int y;

	PontoNaTela(void)
	{
		x = 0;
		y = 0;
	}

	PontoNaTela(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	PontoNaTela(const PontoNaTela* src){
		x = src->x;
		y = src->y;
	}

	bool operator==(const PontoNaTela& other){
		if( this->x == other.x && this->y == other.y)
			return true;
		else
			return false;
	}

	~PontoNaTela(void)
	{
	}
};

