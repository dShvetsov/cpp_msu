#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <MarketPlace.h>
#include <Product.h>
#include <Shop.h>

#include <memory>
#include <exception>

using namespace ::testing;

struct ProductMock : public IProduct {
    MOCK_METHOD1(Attach, void(IShopPtr));
    MOCK_METHOD1(Detach, void(IShopPtr));

    MOCK_CONST_METHOD0(GetPrice, double());
    MOCK_METHOD1(ChangePrice, void(double));

    MOCK_CONST_METHOD0(GetName, const std::string&());
    MOCK_METHOD0(StartSales, void());
    MOCK_METHOD0(StopSales, void());
};

struct ShopMock: public IShop {
    MOCK_METHOD0(Close, void());
    MOCK_CONST_METHOD0(GetName, const std::string&());
    MOCK_CONST_METHOD0(SellAll, IShop::Goods());

    MOCK_METHOD1(DeliverProduct, void(IProductPtr));
    MOCK_METHOD1(PurgeProduct, void(IProductPtr));
    MOCK_METHOD1(OnChangePrice, void(IProductPtr));

    MOCK_METHOD0(GrabNews, IShop::News());
};

TEST(Product, NameTest) {
    IProductPtr product = std::make_shared<Product>("Lasersaber",42.0);
    EXPECT_EQ("Lasersaber", product->GetName());
}

TEST(Shop, NameTest) {
    IShopPtr shop = std::make_shared<Shop>("Tatooine");
    EXPECT_EQ("Tatooine", shop->GetName());
}

TEST(Shop, DeliverPurgeTest) {
    auto product = std::make_shared<ProductMock>();
    EXPECT_CALL(*product, GetPrice())
        .WillRepeatedly(Return(42.0));
    std::string millennim_falcon("Millennium Falcon");
    EXPECT_CALL(*product, GetName())
        .WillRepeatedly(ReturnRef(millennim_falcon));

    IShopPtr shop = std::make_shared<Shop>("Tatooine");

    auto goods = shop->SellAll();
    EXPECT_EQ(0, goods.size()) << "No products at the beggining";

    shop->DeliverProduct(product);
    goods = shop->SellAll();
    ASSERT_EQ(1, goods.size()) << "Exactly one element is selling";
    EXPECT_EQ("Millennium Falcon", goods[0].first);
    EXPECT_DOUBLE_EQ(42, goods[0].second);

    shop->PurgeProduct(product);
    goods = shop->SellAll();
    ASSERT_EQ(0, goods.size()) << "No elements after puring";

    EXPECT_NO_THROW(shop->PurgeProduct(product)) <<  "Do nothing if such products doesn't exist";
}

TEST(Shop, OnChangePriceTest) {
    auto product = std::make_shared<ProductMock>();
    std::string millennim_falcon("Millennium Falcon");
    EXPECT_CALL(*product, GetName())
        .WillRepeatedly(ReturnRef(millennim_falcon));
    EXPECT_CALL(*product, GetPrice())
        .WillRepeatedly(Return(42));

    IShopPtr shop = std::make_shared<Shop>("Tatooine");
    shop->DeliverProduct(product);

    auto news = shop->GrabNews();
    EXPECT_EQ(0, news.size()) << "No news at the begging";

    EXPECT_CALL(*product, GetPrice())
        .WillRepeatedly(Return(451));

    shop->OnChangePrice(product);

    news = shop->GrabNews();
    ASSERT_EQ(1, news.size());
    EXPECT_EQ(millennim_falcon, news[0].product_name);
    EXPECT_DOUBLE_EQ(42, news[0].old_price);
    EXPECT_DOUBLE_EQ(451, news[0].new_price);

    news = shop->GrabNews();
    EXPECT_EQ(0, news.size()) << "News must be erasing after grabbing";

    shop->PurgeProduct(product);
    EXPECT_THROW(shop->OnChangePrice(product), std::out_of_range);
}

TEST(Product, PriceTest) {
    IProductPtr product = std::make_shared<Product>("Lasersaber",42.0);
    EXPECT_DOUBLE_EQ(42.0, product->GetPrice());
    product->ChangePrice(10);
    EXPECT_DOUBLE_EQ(10, product->GetPrice());
}

TEST(Product, ChangePriceTest) {
    IProductPtr product = std::make_shared<Product>("Lasersaber",42.0);
    product->ChangePrice(10);
    EXPECT_DOUBLE_EQ(10, product->GetPrice());
}

TEST(Product, ChangePriceAnnouncmentTest) {
    IProductPtr product = std::make_shared<Product>("Boba Fett's helmet", 220);
    auto shop = std::make_shared<ShopMock>();

    std::string kamino("Kamino");
    EXPECT_CALL(*shop, GetName)
        .WillRepeatedly(ReturnRef(kamino));

    product->Attach(shop);

    EXPECT_CALL(*shop, OnChangePrice(_))
        .Times(0); // No announcment untill sales start
    EXPECT_CALL(*shop, DeliverProduct(_))
        .Times(AtLeast(0));

    product->StartSales();

    EXPECT_CALL(*shop, OnChangePrice(_))
        .Times(1); // Announcment whet sales starts

    product->ChangePrice(340);

}


TEST(Product, StartStopSalesTest) {
    IProductPtr product = std::make_shared<Product>("Boba Fett's helmet", 220);
    auto shop = std::make_shared<ShopMock>();
    std::string kamino("Kamino");
    EXPECT_CALL(*shop, GetName)
        .WillRepeatedly(ReturnRef(kamino));

    EXPECT_CALL(*shop, DeliverProduct(_))
        .Times(0);
    EXPECT_CALL(*shop, PurgeProduct(_))
        .Times(0);

    product->Attach(shop);

    EXPECT_CALL(*shop, DeliverProduct(_))
        .Times(1);

    product->StartSales();

    EXPECT_CALL(*shop, PurgeProduct(_))
        .Times(1);

    product->StopSales();
}

TEST(Product, AttachDetachOnSalesTest) {
    IProductPtr product = std::make_shared<Product>("Boba Fett's helmet", 220);
    auto shop = std::make_shared<ShopMock>();
    std::string kamino("Kamino");
    EXPECT_CALL(*shop, GetName)
        .WillRepeatedly(ReturnRef(kamino));

    EXPECT_CALL(*shop, DeliverProduct(_))
        .Times(0);
    EXPECT_CALL(*shop, PurgeProduct(_))
        .Times(0);

    product->StartSales();

    EXPECT_CALL(*shop, DeliverProduct(_))
        .Times(1);

    product->Attach(shop);

    EXPECT_CALL(*shop, PurgeProduct(_))
        .Times(1);

    product->Detach(shop);
}

