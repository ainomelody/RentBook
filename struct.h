#ifndef STRUCT_H__

#define STRUCT_H__

typedef struct RentInfo				//租书信息
{
	char type;						//图书类别
	char name[31];					//租借者姓名
	char bookNum[12];				//图书编号
	char id[20];					//租借者身份证
	char rentDate[12];				//租书日期
	int keepDays;					//可租天数
	char spReturnDate[12];			//应还日期
	char returnDate[12];			//归还日期
	double additionalPay;			//续费
	struct RentInfo *next;
	struct RentInfo *prev;
}RentInfo;

typedef struct BookInfo				//图书信息
{
	char type;						//图书类别
	char bookName[31];				//图书名称
	char bookNum[10];				//图书编号
	char publisher[30];				//出版社
	char author[20];				//作者
	int stock;						//库存数
	double salePrice;				//售价
	double rentPrice;				//租价
	double vipPrice;				//会员价
	char shelveDate[11];			//上架日期
	char place[5];					//架位
	RentInfo *rentList;				//租书信息
	struct BookInfo *next;
	struct BookInfo *prev;
}BookInfo;

typedef struct BookType				//类别信息
{
	char number;					//编号
	char name[11];					//类别名称
	double payRate;					//续费率
	double vipDiscoutRate;			//会员出租折扣率
	BookInfo *bookList;				//图书信息
	struct BookType *next;
	struct BookType *prev;
}BookType;

#endif
