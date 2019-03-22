#include <klp.connector/klp.connector.hpp>

using namespace eosio;
using namespace std;

namespace eosio {
    sac::sac(account_name self) : eosio::contract(self), _klpmarket(_self, _self), _global(_self, _self) {
//        print(_global.exists());
        get_default_parameters();
        _gstate = _global.exists() ? _global.get() : get_default_parameters();
        auto itr = _klpmarket.find(S(4, KLPCORE));
        if (itr == _klpmarket.end()) {
            auto system_token_supply = 10000000000000ll;
            if (system_token_supply > 0) {
                itr = _klpmarket.emplace(_self, [&](auto &m) {
                    m.supply.amount = 100000000000000ll;
                    m.supply.symbol = S(4, KLPCORE);
                    m.base.balance.amount = int64_t(_gstate.free_klp());
                    m.base.balance.symbol = S(0, KLP);
                    m.quote.balance.amount = system_token_supply / 1000;
                    m.quote.balance.symbol = S(4, EOS);
                });
            }
        } else {
//            print("klp market already created");
        }
    }

    sac::~sac() {
        _global.set(_gstate, _self);
    }

    sac::eosio_global_state sac::get_default_parameters() {
        _global.set(sac::eosio_global_state{68719476736, 0, 0}, N(kylinstokens));
        return _global.get();
    }

    void sac::trytradeklp(uint32_t points, int8_t option) {
        if (option == 0) {
            auto itr = _klpmarket.find(S(4, KLPCORE));
            eosio_assert(itr != _klpmarket.end(), "KLPCORE market not found");
            auto tmp = *itr;
            asset tokens_in = tmp.convert(asset(points, S(0, KLP)), S(4, KLPCORE));
            print(tokens_in);
        } else if (option == 1) {
            eosio_assert(points > 0, "cannot sell negative byte");
            asset tokens_out;
            auto itr = _klpmarket.find(S(4, KLPCORE));
            _klpmarket.modify(itr, 0, [&](auto &es) {
                /// the cast to int64_t of points i=s safe because we certify points is <= quota which is limited by prior purchases
            tokens_out = es.convert(asset(points, S(0, KLP)), S(4, EOS));
            print(tokens_out);
            });
        } else {
            print("Invalid trx");
        }
    }

    int64_t sac::buyklp(asset quant) {
        eosio_assert(quant.amount > 0, "must purchase a positive amount");
        auto fee = quant;
        fee.amount = (fee.amount + 199) / 200; /// .5% fee (round up)
        auto quant_after_fee = quant;
        int64_t points_out;
        auto itr = _klpmarket.find(S(4, KLPCORE));
        _klpmarket.modify(itr, 0, [&](auto &es) {
            /// the cast to int64_t of points is safe because we certify points is <= quota which is limited by prior purchases
            points_out = es.convert(quant_after_fee, S(0, KLP)).amount;
        });
        print(points_out);
        print(uint64_t(points_out));
        print("uint64_t\n\n");
        print(quant_after_fee.amount);
        _gstate.total_klp_points_reserved += uint64_t(points_out);
        _gstate.total_klp_stake += quant_after_fee.amount;
        eosio_assert(points_out > 0, "ensure points reserve more than zero");
        _global.set(_gstate, _self);
        return points_out;
    }

    asset sac::sellklp(int64_t points) {
        eosio_assert(points > 0, "cannot sell negative byte");
        asset tokens_out;
        auto itr = _klpmarket.find(S(4, KLPCORE));
        _klpmarket.modify(itr, 0, [&](auto &es) {
            /// the cast to int64_t of points is safe because we certify points is <= quota which is limited by prior purchases
            tokens_out = es.convert(asset(points, S(0, KLP)), S(4, EOS));
        });
        _gstate.total_klp_points_reserved -= static_cast<decltype(_gstate.total_klp_points_reserved)>(points); // points > 0 is asserted above
        _gstate.total_klp_stake -= tokens_out.amount;
        print("tokens_out");
        print(tokens_out);
        eosio_assert(tokens_out.amount > 0, "token amount received from selling klp is too low");
        _global.set(_gstate, _self);
        return tokens_out;
    }


    void sac::setklp(uint64_t max_klp_size) {
        print("max_klp_size: ");
        print(max_klp_size);

        require_auth(_self);
        eosio_assert(_gstate.max_klp_size < max_klp_size,
                     "ram may only be increased"); /// decreasing ram might result market maker issues
        eosio_assert(max_klp_size < 1024ll * 1024 * 1024 * 1024 * 1024, "ram size is unrealistic");
        eosio_assert(max_klp_size > _gstate.total_klp_points_reserved, "attempt to set max below reserved");

        auto delta = int64_t(max_klp_size) - int64_t(_gstate.max_klp_size);
        auto itr = _klpmarket.find(S(4, KLPCORE));

        /**
         *  Increase or decrease the amount of ram for sale based upon the change in max
         *  ram size.
         */
        _klpmarket.modify(itr, 0, [&](auto &m) {
            m.base.balance.amount += delta;
        });

        _gstate.max_klp_size = max_klp_size;
        _global.set(_gstate, _self);
    }


    void sac::transfer(const account_name sender, const account_name receiver) {
//        eosio_assert(false, "enter transfer");
        array<char, 33> owner_pubkey_char;
        array<char, 33> active_pubkey_char;
        const auto transfer = unpack_action_data<currency::transfer>();
        if (transfer.from == _self || transfer.to != _self) {
            // this is an outgoing transfer, do nothing
            return;
        }
        eosio_assert(transfer.quantity.symbol == string_to_symbol(4, "EOS") ||
                     transfer.quantity.symbol == string_to_symbol(0, "KLP"),
                     "Must be EOS or KLP");
        eosio_assert(transfer.quantity.is_valid(), "Invalid token transfer");
        eosio_assert(transfer.quantity.amount > 0, "Quantity must be positive");



//        eosio_assert(false, "0, \"KLP");
        if (transfer.quantity.symbol == string_to_symbol(0, "KLP")) {
            print("wo shi qilin");
            asset lala = sellklp(transfer.quantity.amount);
            INLINE_ACTION_SENDER(eosio::token, transfer)
                    (N(eosio.token), {{_self, N(active)}},
                     {_self, sender, lala,
                      std::string("Buy KLP points")});
//            eosio_assert(false, "singappre");
            return;
        }


//        eosio_assert(false, "(4, \"EOS");
        if (transfer.quantity.symbol == string_to_symbol(4, "EOS")) {
            asset KLP;
            KLP.symbol = string_to_symbol(0, "KLP");
            KLP.amount = buyklp(transfer.quantity);
            print(KLP.symbol);
            print("baobao");
            print(KLP.amount);
            print(KLP);
//            eosio_assert(false, "pp");
            INLINE_ACTION_SENDER(eosio::token, transfer)
                    (N(kylinsminter), {{_self, N(active)}},
                     {receiver, sender, KLP,
                      std::string("reward KLP to you")});
        }

//        if(transfer.quantity.symbol == string_to_symbol(0, "KLP")){
//            print("wo shi qilin");
//            asset lala = sellklp(transfer.quantity.amount);
//            INLINE_ACTION_SENDER(eosio::token, transfer)
//               (N(klp.token), {{_self, N(active)}},
//                 {_self, sender, lala,
//                    std::string("kylinpoints fee")});
////            eosio_assert(false, "singappre");
//            return;
//        }
//         if(transfer.quantity.symbol == string_to_symbol(4, "EOS")){
//            asset KLP;
//            KLP.symbol = string_to_symbol(0, "KLP");
//            KLP.amount = buyklp(transfer.quantity);
//            print(KLP.symbol);
//            print("baobao");
//            print(KLP.amount);
//            print(KLP);
//            INLINE_ACTION_SENDER(eosio::token, transfer)
//                    (N(klpmintager), {{_self, N(active)}},
//                     {_self, sender, KLP,
//                      std::string("reward KLP to you")});
//        }

    };



//    void
//    sac::regaccount(const account_name sender, const checksum256 hash, const string nonce, const eosio::public_key owner_key,
//               const eosio::public_key active_key) {
//        require_auth(sender);
//
//        orders.emplace(sender, [&](auto &order) {
//            order.id = orders.available_primary_key();
//            order.expires_at = now() + EXPIRE_TIMEOUT;
//            order.hash = hash;
//            order.nonce = nonce;
//            order.owner_key = owner_key;
//            order.active_key = active_key;
//        });
//
//    };
//
//    //@abi action
//    void sac::clearexpired(const account_name sender) {
//        std::vector <order> l;
//
//        // check which objects need to be deleted
//        for (const auto &item : orders) {
//            if (now() > item.expires_at) {
//                l.push_back(item);
//            }
//
//        }
//
//        // delete in second pass
//        for (order item : l) {
//            orders.erase(orders.find(item.primary_key()));
//        }
//    };
//
//    //@abi action
//    void sac::clearall(const account_name sender) {
//        require_auth(_self);
//        std::vector <order> l;
//
//        // check which objects need to be deleted
//        for (const auto &item : orders) {
//            l.push_back(item);
//        }
//
//        // delete in second pass
//        for (order item : l) {
//            orders.erase(orders.find(item.primary_key()));
//        }
//    };
//};


// EOSIO_ABI(sac, (transfer)(regaccount)(clearexpired)(clearall))

#define EOSIO_ABI_EX(TYPE, MEMBERS)                                            \
  extern "C" {                                                                 \
  void apply(uint64_t receiver, uint64_t code, uint64_t action) {              \
    if (action == N(onerror)) {                                                \
      /* onerror is only valid if it is for the "eosio" code account and       \
       * authorized by "eosio"'s "active permission */                         \
      eosio_assert(code == N(eosio), "onerror action's are only valid from "   \
                                     "the \"eosio\" system account");          \
    }                                                                          \
    auto self = receiver;                                                      \
    if (code == self || code == N(eosio.token) || code == N(kylinsminter) || action == N(onerror)  ) {      \
      TYPE thiscontract(self);                                                 \
      switch (action) { EOSIO_API(TYPE, MEMBERS) }                             \
      /* does not allow destructor of thiscontract to run: eosio_exit(0); */   \
    }                                                                          \
  }                                                                            \
  }
    EOSIO_ABI_EX(sac, (transfer)(trytradeklp)(setklp))
}