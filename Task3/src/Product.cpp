#include <Product.h>

#include <algorithm>

Product::Product(std::string name, double price) 
    : m_name(name), m_price(price)
{ }

Product::ShopIterator Product::find_shop(IShopPtr shop) const {
    return std::find_if(m_shops.begin(), m_shops.end(),
        [=](const auto& s) { return s->GetName() == shop->GetName(); });
}

double Product::GetPrice() const {
    return m_price;
}

void Product::ChangePrice(double price) {
    m_price = price;
    if (m_on_sales) {
        for (auto& shop : m_shops) {
            shop->OnChangePrice(shared_from_this());
        }
    }
}

const std::string& Product::GetName() const {
    return m_name;
}

void Product::Attach(IShopPtr shop) {
   auto it = find_shop(shop);
   if (it == m_shops.end()) {
        m_shops.push_back(shop);
        if (m_on_sales) {
            shop->DeliverProduct(shared_from_this());
        }
    }
}

void Product::Detach(IShopPtr shop) {
    auto it = find_shop(shop);
    if (it != m_shops.end()) {
        m_shops.erase(it);
        if (m_on_sales) {
            shop->PurgeProduct(shared_from_this());
        }
    }
}

void Product::StartSales() {
    if (!m_on_sales) {
        for (auto shop : m_shops) {
            shop->DeliverProduct(shared_from_this());
        }
        m_on_sales = true;
    }
}

void Product::StopSales() {
    if (m_on_sales) {
        for (auto shop: m_shops) {
            shop->PurgeProduct(shared_from_this());
        }
        m_on_sales = false;
    }
}
