#include <memory>

#include <atomic>
#include <thread>
#include <chrono>

#include <MarketPlace.h>
#include <Shop.h>
#include <Product.h>

void ShopRoutine(std::vector<IShopPtr> shops)
{
    for (auto& shop: shops) {
        for (auto& good: shop->SellAll()) {
            std::cout << shop->GetName() << ": sell " << good.first << " for " << good.second << "$" <<  std::endl;
        }
    }
    for (auto& shop: shops) {
        for (auto& news: shop->GrabNews()) {
            std::cout << shop->GetName() << " news: " << news.product_name << 
            " change its price from " << news.old_price << "$ to " << news.new_price << "$!" << std::endl;
        }
    }
}

int main() {
    std::atomic<bool> market_is_open;
    market_is_open = true;
    IShopPtr chocolate_factory = std::make_shared<Shop>("Willy Wonko's Chockolate Factory");
    IShopPtr ollivanders = std::make_shared<Shop>("Olliwanders");
    IShopPtr weasles = std::make_shared<Shop> ("Weasleys' Wizard Wheezes");
    IShopPtr florish_and_blotts = std::make_shared<Shop>("Flourish and Blotts");
    IShopPtr broomstix = std::make_shared<Shop>("Broomstix");

    auto prod_thread = std::thread([&]() {
        using namespace std::chrono_literals;
        auto sleep_time = 1s;
        IProductPtr chocolate_frog = std::make_shared<Product>("Chocolate Frogs", 1);
        IProductPtr boosine_wand = std::make_shared<Product>("Boosine Wand", 912321);
        IProductPtr nimbus_2000 = std::make_shared<Product>("Nimbus 2000", 100);
        IProductPtr nimbus_2001 = std::make_shared<Product>("Nimbus 2001", 200);
        IProductPtr firebolt = std::make_shared<Product>("Firebolt", 1000);
        IProductPtr dungbomb = std::make_shared<Product>("Dungbom", 4);
        IProductPtr hats = std::make_shared<Product>("Anti Gravity Hats", 50);
        IProductPtr unopoo = std::make_shared<Product>("U-No-Poo", 25);
        IProductPtr butterbeer = std::make_shared<Product>("Butter Beer", 3);
        IProductPtr dobby_socks = std::make_shared<Product>("Dobby Socks (with the autograph)", 7);
        IProductPtr magical_wand = std::make_shared<Product>("Magical wand", 300);
        IProductPtr peacock_feather = std::make_shared<Product>("Peacock feather", 20);
        IProductPtr beans = std::make_shared<Product>("Every Flavour Beans", 20);

        hats->StartSales();
        dobby_socks->StartSales();
        magical_wand->StartSales();

        boosine_wand->Attach(ollivanders);
        boosine_wand->StartSales();
        chocolate_frog->Attach(chocolate_factory);
        chocolate_frog->StartSales();
        chocolate_frog->Attach(florish_and_blotts);
        nimbus_2000->Attach(broomstix);
        nimbus_2001->Attach(broomstix);
        magical_wand->Attach(ollivanders);
        dungbomb->Attach(weasles);
        beans->Attach(chocolate_factory);
        nimbus_2000->StartSales();
        nimbus_2001->StartSales();
        beans->StartSales();
        beans->Attach(florish_and_blotts);
        std::this_thread::sleep_for(sleep_time);
        chocolate_frog->Detach(chocolate_factory);
        nimbus_2001->ChangePrice(199.99);
        nimbus_2000->ChangePrice(30);
        dungbomb->Attach(chocolate_factory);
        dobby_socks->Attach(weasles);
        dobby_socks->Attach(florish_and_blotts);
        dungbomb->StartSales();
        dobby_socks->Attach(ollivanders);
        hats->Attach(weasles);
        std::this_thread::sleep_for(sleep_time);
        firebolt->Attach(broomstix);
        nimbus_2000->StopSales();
        firebolt->StartSales();
        unopoo->Attach(weasles);
        dobby_socks->StopSales();
        chocolate_frog->StopSales();
        unopoo->StartSales();
        peacock_feather->Attach(ollivanders);
        peacock_feather->StartSales();
        peacock_feather->Attach(broomstix);
        std::this_thread::sleep_for(sleep_time);
        unopoo->StopSales();
        boosine_wand->StopSales();
        firebolt->ChangePrice(1500);
        hats->StopSales();
        std::this_thread::sleep_for(sleep_time);

        butterbeer->StartSales();
        butterbeer->Attach(chocolate_factory);
        butterbeer->Attach(florish_and_blotts);
        butterbeer->Attach(weasles);
        butterbeer->StopSales();
        peacock_feather->StopSales();

        market_is_open = false;
    });

    ShopRoutine({
        chocolate_factory,
        ollivanders,
        weasles,
        florish_and_blotts,
        broomstix
    });

    ShopRoutine({
        chocolate_factory,
        ollivanders,
        weasles,
        florish_and_blotts,
        broomstix
    });

    chocolate_factory->Close();

    while (market_is_open) {
        ShopRoutine({
            chocolate_factory,
            ollivanders,
            weasles,
            florish_and_blotts,
            broomstix
        });
    }
    prod_thread.join();
    return 0;
}
