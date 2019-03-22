#include <eosiolib/eosio.hpp>
using namespace eosio;

class hello : public eosio::contract {
public:
    using contract::contract;

    /// @abi action
    void hi( account_name user ) {
        print( "Hello, ", name{user} );
    }
    void nia( account_name user ) {
        print( "Hello, ", name{user} );
//        eosio_assert(false, "singapore");
        eosio_assert(false, name{user}.to_string().c_str() );
    }
};

EOSIO_ABI( hello, (hi) (nia))