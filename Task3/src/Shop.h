#pragma once

#include <string>
#include <unordered_map>

#include <MarketPlace.h>

class Shop: public IShop {
public:
    Shop(std::string name);
    void Close() override { }
    const std::string& GetName() const override;
    IShop::Goods SellAll() const override;

    void DeliverProduct(IProductPtr product) override;
    void PurgeProduct(IProductPtr product) override;
    void OnChangePrice(IProductPtr product) override;
    IShop::News GrabNews() override;

private:
    struct ProductInfo {
        IProductPtr product;
        double price;
    };
    std::string m_name;
    std::unordered_map<std::string, ProductInfo> m_products;
    News m_news;
};
