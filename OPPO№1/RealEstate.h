#ifndef REALESTATE_H
#define REALESTATE_H

#include <string>

class RealEstate {
public:
	void SetProperty(const std::string& prop);
	void SetOwner(const std::string& own);
	void SetDate(const std::string& dt);
	void SetPrice(const std::string& pr);

	const std::string& GetProperty() const;
	const std::string& GetOwner() const;
	const std::string& GetDate() const;
	int GetPrice() const;

	void Print() const;
	bool IsComplete() const;
	void Clear();

private:
	std::string property_;
	std::string owner_;
	std::string date_;
	int price_int_ = 0;
	bool price_set_ = false;
};

#endif  