#include "greencoins.hpp"


void greencoins::issue( account_name user, account_name org, uint64_t action, std::string memo) {

      require_auth(org);
      auto act = actions.find(action);
      eosio_assert(act != actions.end(), "no existing action");

      auto reward_amount = act->amount;

      adjust_balance(user, reward_amount);
      print(name{user}, " got green coins from, ", memo);

}

void greencoins::createaction( account_name org, int64_t amount, std::string action_content) {

      require_auth(org);

      auto id = actions.available_primary_key();

      actions.emplace(org, [&](auto &a) {
          a.id = id;
          a.org = org;
          a.amount = amount;
          a.content = action_content;
      });
      print(name{org}, " created action <", action_content,">");
}

void greencoins::rmaction( int64_t action ) {

      auto act = actions.find(action);

      eosio_assert(act != actions.end(), "no existing action");

      require_auth(act->org);
      actions.erase(act);

}

void greencoins::commit( account_name com, int64_t amount) {

      require_auth(com);
      eosio_assert(amount > 0, "cannot commit negative amount of tokens");

      adjust_commits(com, amount);

    print(name{com}, " commit ", amount, "green coins");
}

void greencoins::redeem( account_name user, account_name com, int64_t amount, std::string memo) {

      require_auth(user);
      adjust_balance(user, amount);
      adjust_commits(com, amount);

    print(name{user}, " redeemed ", -amount, "green coins on ", memo);


}

void greencoins::adjust_balance( account_name user, int64_t delta) {

      auto u = balances.find( user );

      if ( u != balances.end() ) {
            eosio_assert(u->amount+delta >= 0, "user does not have enough green points!");
            balances.modify( u, 0, [&]( auto &bal ){
                bal.amount += delta;
            });
      } else {
            eosio_assert(delta > 0, "cannot obtain non positive amount!");
            balances.emplace( user, [&]( auto &bal ){
                bal.user = user;
                bal.amount = delta;
            });
      }

}

void greencoins::adjust_commits( account_name com, int64_t delta) {

      auto comp = contributes.find( com );

      if ( comp != contributes.end() ) {
            eosio_assert(comp->amount+delta >= 0, "this company does not have enough green quota!");
            contributes.modify( comp, 0, [&]( auto &co ){
                co.amount += delta;
            });
      } else {
            eosio_assert(delta > 0, "cannot commit non positive amount!");
            contributes.emplace( com, [&]( auto &co ){
                co.com = com;
                co.amount = delta;
            });
      }

}


EOSIO_ABI( greencoins, (issue)(createaction)(rmaction)(commit)(redeem) )
