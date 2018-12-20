#pragma once
#include <iostream>
#include <memory>
#include <tuple>
#include <string>
#include <vector>

using IShopPtr = std::shared_ptr<class IShop>;
using IProductPtr = std::shared_ptr<class IProduct>;

class IProduct: public std::enable_shared_from_this<IProduct> {
public:
    virtual void Attach(IShopPtr shop) = 0;
    virtual void Detach(IShopPtr shop) = 0;

    virtual double GetPrice() const = 0;
    virtual void ChangePrice(double price) = 0;

    virtual const std::string& GetName() const = 0;

    virtual void StartSales() = 0;
    virtual void StopSales() = 0;
    virtual ~IProduct() = default;
};

class IShop {
public:
    using Good = std::pair<std::string, double>;
    using Goods = std::vector<Good>;

    struct NewsNote {
        NewsNote(std::string product_name, double old_price, double new_price)
            : product_name(product_name), old_price(old_price), new_price(new_price)
        { }
        std::string product_name;
        double old_price;
        double new_price;
    };

    using News = std::vector<NewsNote>;

    virtual void Close() = 0;
    virtual const std::string& GetName() const = 0;
    virtual Goods SellAll() const = 0;

    virtual void DeliverProduct(IProductPtr product) = 0;
    virtual void PurgeProduct(IProductPtr product) = 0;
    virtual void OnChangePrice(IProductPtr product) = 0;

    virtual News GrabNews() = 0;
};

