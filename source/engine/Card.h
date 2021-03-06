#pragma once

#include "Common.h"
#include "CardType.h"
#include "rapidjson/rapidjson.h"

namespace Prismata
{
    enum CardRoles { };
 
namespace CardCreationMethod
{
    enum { Bought, BuyScript, AbilityScript, Manual };
}

namespace AliveStatus
{
    enum { Alive, Dead, KilledThisTurn };
}

namespace CauseOfDeath
{
    enum { None, SelfSac, SelfAbilityHealthCost, Sniped, BuySacCost, AbilitySacCost, Blocker, Breached, Lifespan, UndoCreate, Unknown, Deleted, NumCausesOfDeath};
}

namespace DamageSource
{
    enum { Block, Breach, NumDamageSources };
}

class Card
{
    CardType            _type;                  
    CardID              _id                 = (CardID)-1;                    
    CardID              _targetID           = 0;              
    PlayerID            _player             = Players::Player_None;                
    HealthType          _currentHealth      = 0;         
    HealthType          _currentChill       = 0;
    HealthType          _damageTaken        = 0;
    TurnType            _constructionTime   = 0;
    TurnType            _currentDelay       = 0;
    TurnType            _lifespan           = 0;
    ChargeType          _currentCharges     = 0;
    EnumType            _status             = CardStatus::Default;
    EnumType            _aliveStatus        = AliveStatus::Alive;
    EnumType            _causeOfDeath       = CauseOfDeath::None;
    
    bool                _hasTarget          = false;
    bool                _dead               = true;
    bool                _sellable           = false;
    bool                _inPlay             = false;
    bool                _wasBreached        = false;
    bool                _abilityUsedThisTurn = false;
    
    std::vector<CardID> _killedCardIDs;
    std::vector<CardID> _createdCardIDs;
    
public:
 
    Card();
    Card(const std::string & jsonString);
    Card(const rapidjson::Value & cardValue);
    Card(const CardType & type, const PlayerID & player, const EnumType & creationMethod, const TurnType & delay, const TurnType & lifespan);
    Card & operator = (const Card & rhs);
    bool          operator == (const Card & rhs) const;
    bool          operator < (const Card & rhs) const;

    const CardType &    getType() const;
    const CardID &      getID() const;
    const CardID &      getTargetID() const;
    const PlayerID &    getPlayer() const;
    const HealthType &  currentHealth() const;
    const HealthType &  currentChill() const;
    const HealthType &  getDamageTaken() const;
    const EnumType &    getAliveStatus() const;
    const ChargeType &  getCurrentCharges() const;
    const TurnType &    getConstructionTime() const;
    const TurnType &    getCurrentLifespan() const;
    const TurnType &    getCurrentDelay() const;
    const EnumType &    getStatus() const;
    
    bool canBlock() const;
    bool isUnderConstruction() const;
    bool isDead() const;
    bool canUseAbility() const;
    bool canUndoUseAbility() const;
    bool canRunBeginOwnTurnScript() const;
    bool canSac() const;
    bool isBreachable() const;
    bool isDelayed() const;
    bool meetsCondition(const Condition & condition) const;
    bool isIsomorphic(const Card & other) const;
    bool canBreachFor(const HealthType & damage) const;
    bool isOverkillable() const;
    bool canOverkillFor(const HealthType & damage) const;
    bool canBeChilled() const;
	bool canFrontlineFor(const HealthType & damagee) const;
    bool canBlockOnly() const;
    bool isSellable() const;
    bool isInPlay() const;
    bool isFrozen() const;
    bool hasTarget() const;
    bool selfKilled() const;
    bool wasBreached() const;

    void setStatus(EnumType status);
    void takeDamage(const HealthType & amount, const EnumType & damageSource);
    void toggleAssigned();
    void useAbility();
    void undoUseAbility();
    void runAbilityScript();
    void runBeginTurnScript();
    void beginTurn();
    void kill(const EnumType & causeOfDeath);
    void applyChill(const HealthType & amount);
    void removeChill(const HealthType & amount);
    void setID(const CardID & id);
    void addKilledCardID(const CardID & id);
    void addCreatedCardID(const CardID & id);
    void undoKill();
    void endTurn();
    void setTargetID(const CardID & targetID);
    void setInPlay(bool inPlay);
    void clearTarget();
    void undoBreach();

    const std::vector<CardID> & getKilledCardIDs() const;
    const std::vector<CardID> & getCreatedCardIDs() const;

    const std::string toJSONString(bool formatted = false) const;
};

class IsomorphicCardComparator 
{
    const Card & _card;

public:

    IsomorphicCardComparator(const Card & card)
        : _card(card)
    {
    }

    bool operator() (const Card & c) const
    {
        return c.isIsomorphic(_card);
    }
};
}