#include <algorithm>
#include <cmath>
#include <eosiolib/asset.hpp>
#include <eosiolib/currency.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/public_key.hpp>
#include "eosio.token.hpp"
#include "exchange_state.cpp"
#include "exchange_state.hpp"
#include "abieos_numeric.hpp"
#include <eosiolib/crypto.h>
#include <eosiolib/singleton.hpp>
//#include "klpmarket.cpp"
using namespace eosio;
namespace eosio {
    struct permission_level_weight {
        permission_level permission;
        weight_type weight;

        // explicit serialization macro is not necessary, used here only to improve
        // compilation time
        EOSLIB_SERIALIZE(permission_level_weight, (permission)(weight))
    };

    struct key_weight {
        eosio::public_key key;
        weight_type weight;

        // explicit serialization macro is not necessary, used here only to improve
        // compilation time
        EOSLIB_SERIALIZE(key_weight, (key)(weight))
    };

    struct wait_weight {
        uint32_t wait_sec;
        weight_type weight;

        // explicit serialization macro is not necessary, used here only to improve
        // compilation time
        EOSLIB_SERIALIZE(wait_weight, (wait_sec)(weight))
    };

    struct authority {
        uint32_t threshold;
        vector <key_weight> keys;
        vector <permission_level_weight> accounts;
        vector <wait_weight> waits;

        // explicit serialization macro is not necessary, used here only to improve
        // compilation time
        EOSLIB_SERIALIZE(authority, (threshold)(keys)(accounts)(waits))
    };

    struct call {
        struct eosio {
            void newaccount(account_name creator, account_name name, authority owner,
                            authority active);

            void delegatebw(account_name from, account_name receiver,
                            asset stake_net_quantity, asset stake_cpu_quantity,
                            bool transfer);

            void buyram(account_name buyer, account_name receiver, asset tokens);
        };
    };

//    asset buyrambytes(uint32_t bytes) {
//        rammarket market(N(eosio), N(eosio));
//        auto itr = market.find(S(4, RAMCORE));
//        eosio_assert(itr != market.end(), "RAMCORE market not found");
//        auto tmp = *itr;
//        return tmp.convert(asset(bytes, S(0, RAM)), CORE_SYMBOL);
//    }



    class sac : public contract {
    private:
        struct eosio_global_state
         {
            uint64_t free_klp() const { return max_klp_size - total_klp_points_reserved; }
            uint64_t max_klp_size = 64ll * 1024 * 1024 * 1024;
            uint64_t total_klp_points_reserved = 0;
            int64_t total_klp_stake = 0;
            EOSLIB_SERIALIZE( eosio_global_state, (max_klp_size)(total_klp_points_reserved)(total_klp_stake)
            )
        };
        typedef eosio::singleton<N(global), eosio_global_state> global_state_singleton;

        eosio::klpmarket _klpmarket;
        eosio_global_state     _gstate;
        global_state_singleton _global;



    public:
        sac( account_name s );
        ~sac();
        eosio_global_state get_default_parameters();
        int64_t buyklp(asset quant);
        asset sellklp( int64_t points );
        void setklp( uint64_t max_klp_size );
        const uint32_t EXPIRE_TIMEOUT = 60 * 60 * 3;
        void transfer(const account_name sender, const account_name receiver);
        void trytradeklp(uint32_t points, int8_t option);

    };

}
