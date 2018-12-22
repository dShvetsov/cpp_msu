#pragma once

#include <atomic>
#include <string>
#include <mutex>

#include <MarketPlace.h>

class Product: public IProduct {
public:
    Product(std::string name, double price);
    void Attach(IShopPtr shop) override;
    void Detach(IShopPtr shop) override;

    double GetPrice() const override;
    void ChangePrice(double price) override;

    const std::string& GetName() const override;

    void StartSales() override;
    void StopSales() override;
private:
    std::string m_name;
    double m_price;
    std::vector<IShopPtr> m_shops;
    using ShopIterator = std::vector<IShopPtr>::const_iterator;
    std::atomic<bool> m_on_sales{false};

    ShopIterator find_shop(IShopPtr) const;
    mutable std::mutex mutex;

};
