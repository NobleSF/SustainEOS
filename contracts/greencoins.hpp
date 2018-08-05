/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#ifndef GREEN_HPP
#define GREEN_HPP

#include <eosiolib/eosio.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/multi_index.hpp>

using namespace eosio;

class greencoins {

private:
    account_name      _this_contract;

public:
    // @abi action
    void issue( account_name user, account_name org, uint64_t action, std::string memo);

    // @abi action
    void createaction( account_name org, int64_t amount, std::string action_content);

    // @abi_action
    void rmaction( int64_t action);

    // @abi action
    void commit(account_name com, int64_t amount);

    // @abi action
    void redeem( account_name user, account_name com, int64_t amount, std::string memo);

    void adjust_balance( account_name user, int64_t delta);

    void adjust_commits( account_name com, int64_t delta);

        // @abi table
    struct balance {
        account_name user;
        int64_t amount;

        auto primary_key()const { return user; }

        EOSLIB_SERIALIZE( balance, (user)(amount) )
    };

    typedef multi_index<N(balances), balance> balance_table;

    // @abi table
    struct contribution {
        account_name com;
        uint64_t amount;
        auto primary_key()const { return com; }

        EOSLIB_SERIALIZE( contribution, (com)(amount) )
    };

    typedef multi_index<N(contributes), contribution> contribution_table;

    // @abi table
    struct action {
        int64_t id;
        account_name org;
        uint64_t amount;
        std::string content;
        auto primary_key()const { return id; }

        EOSLIB_SERIALIZE( action, (id)(org)(amount)(content) )
    };

    typedef multi_index<N(actions), action> action_table;

    balance_table balances;
    contribution_table contributes;
    action_table actions;

public:
    greencoins(account_name self) : _this_contract(self),
                                    balances(self, self),
                                    contributes(self, self),
                                    actions(self, self)
    {}

};
#endif
