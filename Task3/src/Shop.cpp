#include <Shop.h>

#include <algorithm>
#include <exception>

Shop::Shop(std::string name) : m_name(name) 
{ }

const std::string& Shop::GetName() const {
    return m_name;
}

IShop::Goods Shop::SellAll() const {
    Goods goods;
    goods.reserve(m_products.size());
    for (auto p : m_products) {
        goods.emplace_back(p.second.product->GetName(), p.second.product->GetPrice());
    }
    return goods;
}

void Shop::DeliverProduct(IProductPtr product) {
    m_products[product->GetName()] = ProductInfo{product, product->GetPrice()};
}

void Shop::PurgeProduct(IProductPtr product_for_purge) {
    m_products.erase(product_for_purge->GetName());
}

IShop::News Shop::GrabNews() {
    News ret;
    ret = std::move(m_news);
    return ret;
}

void Shop::OnChangePrice(IProductPtr product) {
    auto& product_info = m_products.at(product->GetName());
    m_news.emplace_back(product->GetName(), product_info.price, product->GetPrice());
    product_info.price = product->GetPrice();
}
