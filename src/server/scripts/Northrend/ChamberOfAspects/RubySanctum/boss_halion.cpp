/*
 * Copyright (C) 2008 - 2011 Trinity <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
 
 
 /* Original de Vlad
 * Adcicionales y mejoras por Ws
 * Ajustes/Complementos y limpieza por Eilo (https://github.com/eilo)
 * WowRean Script www.wowrean.com
 *
 * Esta script funciona las zonas de vacio cuando dispelea el pj el combustion y el consuption
 * ademas tiene el tema de no seleccionar al tank para el meteoro y combustion/consumption tambien
 * al final se ejecuta un mini evento con unos dragones volando para darle mayor lore a la instance
 */

#include "ScriptPCH.h"
#include "ruby_sanctum.h"

enum
{
    //SPELLS
    //All
    SPELL_TWILIGHT_PRECISION                    = 78243, // Increases Halion's chance to hit by 5% and decreases all players' chance to dodge by 20%
    SPELL_BERSERK                               = 26662, // Increases the caster's attack and movement speeds by 150% and all damage it deals by 500% for 5 min.  Also grants immunity to Taunt effects.
    SPELL_START_PHASE2                          = 74808, // Phases the caster into the Twilight realm, leaving behind a large rift.
    SPELL_TWILIGHT_ENTER                        = 74807, // Phases the caster into the Twilight realm - phase 32
    SPELL_TWILIGHT_ENTER2                       = 74812, //
    SPELL_SUMMON_TWILIGHT_PORTAL                = 74809, //

    SPELL_FIRE_PILLAR                           = 76006, // Visual intro
    SPELL_FIERY_EXPLOSION                       = 76010, // Visual intro

    //NEED SCRIPT
    SPELL_TAIL_LASH                             = 74531, // A sweeping tail strike hits all enemies behind the caster, inflicting 3063 to 3937 damage and stunning them for 2 sec.
    SPELL_TWILIGHT_DIVISION                     = 75063, // Phases the caster, allowing him to exist and act simultaneously in both the material and Twilight realms.
    SPELL_TWILIGHT_CUTTER                       = 77844, // 68114  77844 74769dont work (( Inflicts 13,875 to 16,125 Shadow damage every second to players touched by the shadow beam
    SPELL_TWILIGHT_CUTTER_CHANNEL               = 74768, // Visual effect triggers 74769
    //CORPOREALITY
    SPELL_CORPOREALITY_EVEN                     = 74826, // Deals & receives normal damage
    SPELL_CORPOREALITY_20I                      = 74827, // Damage dealt increased by 10% & Damage taken increased by 15%
    SPELL_CORPOREALITY_40I                      = 74828, // Damage dealt increased by 30% & Damage taken increased by 50%
    SPELL_CORPOREALITY_60I                      = 74829, // Damage dealt increased by 60% & Damage taken increased by 100%
    SPELL_CORPOREALITY_80I                      = 74830, // Damage dealt increased by 100% & Damage taken increased by 200%
    SPELL_CORPOREALITY_100I                     = 74831, // Damage dealt increased by 200% & Damage taken increased by 400%
    SPELL_CORPOREALITY_20D                      = 74832, // Damage dealt reduced by 10% & Damage taken reduced by 15%
    SPELL_CORPOREALITY_40D                      = 74833, // Damage dealt reduced by 30% & Damage taken reduced by 50%
    SPELL_CORPOREALITY_60D                      = 74834, // Damage dealt reduced by 60% & Damage taken reduced by 100%
    SPELL_CORPOREALITY_80D                      = 74835, // Damage dealt reduced by 100% & Damage taken reduced by 200%
    SPELL_CORPOREALITY_100D                     = 74836, // Damage dealt reduced by 200% & Damage taken reduced by 400%
    //METEOR STRIKE
    SPELL_METEOR                                = 74637, // Script Start (summon NPC_METEOR_STRIKE)
    SPELL_METEOR_IMPACT                         = 74641, // IMPACT ZONE FOR METEOR
    SPELL_METEOR_STRIKE                         = 74648, // Inflicts 18,750 to 21,250 Fire damage to enemies within 12 yards of the targeted area. Takes about 5 seconds to land.
    SPELL_METEOR_FLAME                          = 74718, // FLAME FROM METEOR
    //N10
    SPELL_FLAME_BREATH                          = 74525, // Inflicts 17,500 to 22,500 Fire damage to players in front of Halion
    SPELL_DARK_BREATH                           = 74806, // Inflicts 17,500 to 22,500 Shadow damage to players in front of Halion
    SPELL_DUSK_SHROUD                           = 75484, // Inflicts 3,000 Shadow damage every 2 seconds to everyone in the Twilight Realm
    //Combustion
    NPC_COMBUSTION                              = 40001,
    SPELL_MARK_OF_COMBUSTION                    = 74567, // Dummy effect only
    SPELL_FIERY_COMBUSTION                      = 74562, // Inflicts 4,000 Fire damage every 2 seconds for 30 seconds to a random raider. Every time Fiery Combustion does damage, it applies a stackable Mark of Combustion.
    SPELL_COMBUSTION_EXPLODE                    = 74607,
    SPELL_COMBUSTION_AURA                       = 74629,
    //Consumption
    NPC_CONSUMPTION                             = 40135,
    SPELL_MARK_OF_CONSUMPTION                   = 74795, // Dummy effect only
    SPELL_SOUL_CONSUMPTION                      = 74792, // Inflicts 4,000 Shadow damage every 2 seconds for 30 seconds to a random raider. Every time Soul Consumption does damage, it applies a stackable Mark of Consumption.
    SPELL_CONSUMPTION_EXPLODE                   = 74799,
    SPELL_CONSUMPTION_AURA                      = 74803,
    //Summons
    NPC_METEOR_STRIKE                           = 40029, //casts "impact zone" then meteor
    NPC_METEOR_STRIKE_1                         = 40041,
    NPC_METEOR_STRIKE_2                         = 40042,

    FR_RADIUS                                   = 45,

    //SAYS
    SAY_HALION_SPAWN                = -1666100, //17499 Meddlesome insects, you're too late! The Ruby Sanctum is lost.
    SAY_HALION_AGGRO                = -1666101, //17500 Your world teeters on the brink of annihilation. You will all bear witness to the coming of a new age of destruction!
    SAY_HALION_SLAY_1               = -1666102, //17501 Another hero falls.
    SAY_HALION_SLAY_2               = -1666103, //17502 Ha Ha Ha!
    SAY_HALION_DEATH                = -1666104, //17503 Relish this victory mortals, for it will be your last. This world will burn with the Master's return!
    SAY_HALION_BERSERK              = -1666105, //17504 Not good enough!
    SAY_HALION_SPECIAL_1            = -1666106, //17505 The heavens burn!
    SAY_HALION_SPECIAL_2            = -1666107, //17506 Beware the shadow!
    SAY_HALION_PHASE_2              = -1666108, //17507 You will find only suffering within the realm of Twilight. Enter if you dare.
    SAY_HALION_PHASE_3              = -1666109, //17508 I am the light AND the darkness! Cower mortals before the Herald of Deathwing!
    EMOTE_WARNING                   = -1666110, //orbs charge warning
    EMOTE_REAL                      = -1666111, // To real world message
    EMOTE_TWILIGHT                  = -1666112, // To twilight world message
    EMOTE_NEITRAL                   = -1666113, // Halion reveal HP message
};

struct Locations
{
    float x, y, z;
};

static Locations SpawnLoc[]=
{
    {3154.99f, 535.637f, 72.8887f},            // 0 - Halion spawn point (center)
};

class boss_halion_real : public CreatureScript
{
public:
    boss_halion_real() : CreatureScript("boss_halion_real") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_halion_realAI(pCreature);
    }

    struct boss_halion_realAI : public ScriptedAI
    {
        boss_halion_realAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        bool intro;
        uint8 stage;
        uint8 nextPoint;
        uint32 m_uiIntroTimer;
        uint32 m_uiIntroAppTimer;
        uint32 m_uiEnrage;
        uint32 m_uiFlameTimer;
        uint32 m_uiFieryTimer;
        uint32 m_uiMeteorTimer;
        uint32 m_uiTailLashTimer;
        bool MovementStarted;

        void Reset()
        {
            if(!instance)
                return;

            me->SetRespawnDelay(7*DAY);

            if (me->isAlive())
            {
                instance->SetData(TYPE_HALION, NOT_STARTED);
                instance->SetData(TYPE_HALION_EVENT, FAIL);
				me->SetDisplayId(11686);   
            }

            setStage(0);
            nextPoint = 0;
            intro = false;
            m_uiIntroTimer = 1*IN_MILLISECONDS;
            m_uiIntroAppTimer = 30*IN_MILLISECONDS;
            m_uiEnrage = 600*IN_MILLISECONDS;
            m_uiFlameTimer = urand(10*IN_MILLISECONDS,18*IN_MILLISECONDS);
            m_uiFieryTimer = urand(30*IN_MILLISECONDS,40*IN_MILLISECONDS);
            m_uiMeteorTimer = urand(30*IN_MILLISECONDS,35*IN_MILLISECONDS);
            m_uiTailLashTimer = urand(15*IN_MILLISECONDS,25*IN_MILLISECONDS);

            SetCombatMovement(true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);

            if (GameObject* pGoPortal = me->FindNearestGameObject(GO_HALION_PORTAL_1, 50.0f))
                pGoPortal->Delete();
            if (GameObject* pGoPortal = me->FindNearestGameObject(GO_HALION_PORTAL_2, 50.0f))
                pGoPortal->Delete();
            if (GameObject* pGoPortal = me->FindNearestGameObject(GO_HALION_PORTAL_3, 50.0f))
                pGoPortal->Delete();
        }

        void setStage(uint8 phase)
        {
            stage = phase;
        }

        uint8 getStage()
        {
            return stage;
        }

        void MoveInLineOfSight(Unit* pWho)
        {
            if (!instance)
                return;

            if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER)
                return;

            if (!intro && pWho->IsWithinDistInMap(me, 80.0f))
                {
                    DoScriptText(-1666100,me);
                    intro = true;
                }

            if (intro && !me->isInCombat() && pWho->IsWithinDistInMap(me, 30.0f))
            {
               // AttackStart(pWho);
               me->SetInCombatWith(pWho);
               SetCombatMovement(false);
               me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_DISABLE_MOVE);
               me->GetMotionMaster()->MoveIdle();
               setStage(10);
            }

        }

        void EnterEvadeMode()
        {
            if (!instance)
                return;

            if (instance->GetData(TYPE_HALION_EVENT) != FAIL)
                return;

            ScriptedAI::EnterEvadeMode();
        }

        void JustDied(Unit* pKiller)
        {
            if (!instance)
                return;

            DoScriptText(-1666104,me);

            Creature* pclone = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_TWILIGHT));

            if (!pclone || !pclone->isAlive())
            {

                instance->SetData(TYPE_HALION, DONE);
				//invoca dragones <HALION> al termino del evento. WS-CORE.
				me->SummonCreature(NPC_DRAGON, 3236.96f, 556.85f, 113.36f, 3.32f);
				me->SummonCreature(NPC_DRAGON, 3229.16f, 497.82f, 111.209f, 2.7f);
				me->SummonCreature(NPC_DRAGON, 3193.10f, 533.63f, 103.49f, 3.14f);
				me->SummonCreature(NPC_DRAGON, 3153.33f, 574.20f, 97.13f, 4.8f);
				me->SummonCreature(NPC_DRAGON, 3151.94f, 492.30f, 97.28f, 1.5f);
				me->SummonCreature(NPC_DRAGON, 3119.94f, 535.65f, 100.36f, 6.2f);
				me->isAlive();
                me->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                instance->SetData(TYPE_COUNTER, 0);
            }
            else
            {
                me->RemoveFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
            }
        }

        void KilledUnit(Unit* pVictim)
        {
            switch (urand(0,1))
            {
                case 0:
                    DoScriptText(-1631006,me,pVictim);
                    break;
                case 1:
                    DoScriptText(-1631007,me,pVictim);
                    break;
            }
        }

        void EnterCombat(Unit* pWho)
        {
            if (!instance)
                return;

            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            DoCast(SPELL_TWILIGHT_PRECISION);
            me->SetInCombatWithZone();
            instance->SetData(TYPE_HALION, IN_PROGRESS);
            DoScriptText(-1666101, me);
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (!instance)
                return;

            if (type != POINT_MOTION_TYPE || !MovementStarted)
                return;

            if (id == nextPoint)
            {
                me->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
            }
        }

        void StartMovement(uint32 id)
        {
            nextPoint = id;
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MovePoint(id, SpawnLoc[id].x, SpawnLoc[id].y, SpawnLoc[id].z);
            MovementStarted = true;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!instance)
                return;

            if (!UpdateVictim())
                return;

            switch (getStage())
            {
                case 10: //PHASE INTRO
                    if (m_uiIntroTimer <= uiDiff)
                    {
                        DoCast(SPELL_FIRE_PILLAR);
                        m_uiIntroAppTimer = 10*IN_MILLISECONDS;
                        m_uiIntroTimer = 30*IN_MILLISECONDS;
                    } else m_uiIntroTimer -= uiDiff;

                    if (m_uiIntroAppTimer <= uiDiff)
                    {
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_DISABLE_MOVE);
                        DoCast(SPELL_FIERY_EXPLOSION);
                        me->SetDisplayId(31952);
                        SetCombatMovement(true);
                        me->GetMotionMaster()->MoveChase(me->getVictim());
                        setStage(0);
                        m_uiIntroAppTimer = 30*IN_MILLISECONDS;
                    } else m_uiIntroAppTimer -= uiDiff;

                    break;

                case 0: //PHASE 1 PHYSICAL REALM
                    if (m_uiFlameTimer <= uiDiff)
                    {
                        DoCast(me, SPELL_FLAME_BREATH);
                        m_uiFlameTimer = urand(12*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiFlameTimer -= uiDiff;

                    if (m_uiTailLashTimer <= uiDiff)
                    {
                        DoCast(SPELL_TAIL_LASH);
                        m_uiTailLashTimer = urand(10*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiTailLashTimer -= uiDiff;

                    if (m_uiMeteorTimer <= uiDiff)
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 45.0f, true);
                        if (target)
                        {
                            DoCast(target, SPELL_METEOR);
                            m_uiMeteorTimer = urand(30*IN_MILLISECONDS,35*IN_MILLISECONDS);
                        }
                    } else m_uiMeteorTimer -= uiDiff;

                    if (m_uiFieryTimer <= uiDiff)
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 45.0f, true);
                        if (target)
                        {
                            DoCast(target, SPELL_FIERY_COMBUSTION);
                            m_uiFieryTimer = urand(25*IN_MILLISECONDS,40*IN_MILLISECONDS);
                        }
                    } else m_uiFieryTimer -= uiDiff;

                    if ( HealthBelowPct(76))
                    {
                        setStage(1);
                        me->AttackStop();
                        me->InterruptNonMeleeSpells(true);
                        SetCombatMovement(false);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    }
                    break;

                case 1: // Switch to phase 2
                    {
                        DoScriptText(-1666108,me);
                        instance->SetData(TYPE_HALION_EVENT, NOT_STARTED);
                        StartMovement(0);
                            Creature* pControl = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_CONTROL));
                            if (!pControl)
                                pControl = me->SummonCreature(NPC_HALION_CONTROL, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 1000);
                            else if (!pControl->isAlive())
                                pControl->Respawn();
                            pControl->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->SetInCombatWith(pControl);
                            pControl->SetInCombatWith(me);
                        setStage(2);
                        break;
                    }
                case 2:
                    if (MovementStarted)
                        return;

                    DoCast(me, SPELL_SUMMON_TWILIGHT_PORTAL);
                    setStage(3);
                    if (GameObject* pGoPortal = instance->instance->GetGameObject(instance->GetData64(GO_HALION_PORTAL_1)))
                        pGoPortal->SetPhaseMask(31,true);
					if (GameObject* pGoRing = instance->instance->GetGameObject(instance->GetData64(GO_FLAME_CREP)))
					pGoRing->SetPhaseMask(65535,true);
					//Ahora se queman los arboles < script HALION> WS-CORE.
                    if (GameObject* pGO = GameObject::GetGameObject(*me, instance->GetData64(GO_ARBOL)))
                        instance->HandleGameObject(pGO->GetGUID(),true);

                    if (GameObject* pGO = GameObject::GetGameObject(*me, instance->GetData64(GO_ARBOL1)))
                        instance->HandleGameObject(pGO->GetGUID(),true);

                    if (GameObject* pGO = GameObject::GetGameObject(*me, instance->GetData64(GO_ARBOL2)))
                        instance->HandleGameObject(pGO->GetGUID(),true);

                    if (GameObject* pGO = GameObject::GetGameObject(*me, instance->GetData64(GO_ARBOL3)))
                        instance->HandleGameObject(pGO->GetGUID(),true); 

                    break;

                case 3:
                    if (me->IsNonMeleeSpellCasted(false))
                        return;
                    DoCast(SPELL_START_PHASE2);
                    setStage(4);
                    break;

                case 4:
                    if (!me->IsNonMeleeSpellCasted(false))
                    {
                        if (Creature* pControl = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_CONTROL)))
                        {
                            me->SetInCombatWith(pControl);
                            pControl->SetInCombatWith(me);
                        }
                        Creature* pTwilight = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_TWILIGHT));
                        if (!pTwilight)
                            pTwilight = me->SummonCreature(NPC_HALION_TWILIGHT, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 1000);
                        else if (!pTwilight->isAlive())
                            pTwilight->Respawn();
                        pTwilight->SetCreatorGUID(0);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        setStage(5);
                    }
                    break;
                case 5: // HALION awaiting end battle in TWILIGHT REALM
                    if (instance->GetData(TYPE_HALION_EVENT) == IN_PROGRESS)
                        {
//                            instance->SetData(TYPE_HALION_EVENT, SPECIAL);
                            me->RemoveAurasDueToSpell(SPELL_START_PHASE2);
                            if (Creature* pControl = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_CONTROL)))
                            {
                                me->SetInCombatWith(pControl);
                                pControl->SetInCombatWith(me);
                            }
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            me->SetHealth(me->GetMaxHealth()/2);
                            me->SetInCombatWithZone();
                            setStage(6);
                        }
                    return;

                case 6: // Switch to phase 3
//                    DoCast(SPELL_TWILIGHT_DIVISION);
                    DoScriptText(-1666109,me);
                    instance->SetData(TYPE_HALION_EVENT, SPECIAL);
                    setStage(7);
                    break;

                case 7:
                    if (me->IsNonMeleeSpellCasted(false))
                        return;
                    if (me->getVictim()->GetTypeId() != TYPEID_PLAYER)
                        return;
                    SetCombatMovement(true);
                    me->GetMotionMaster()->MoveChase(me->getVictim());
                    setStage(8);
                    break;

                case 8: //PHASE 3 BOTH REALMS
                    if (m_uiFlameTimer <= uiDiff)
                    {
                        DoCast(SPELL_FLAME_BREATH);
                        m_uiFlameTimer = urand(12*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiFlameTimer -= uiDiff;

                    if (m_uiTailLashTimer <= uiDiff)
                    {
                        DoCast(SPELL_TAIL_LASH);
                        m_uiTailLashTimer = urand(10*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiTailLashTimer -= uiDiff;

                    if (m_uiMeteorTimer <= uiDiff)
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 45.0f, true);
                        if (target)
                        {
                            DoCast(target, SPELL_METEOR);
                            m_uiMeteorTimer = urand(30*IN_MILLISECONDS,35*IN_MILLISECONDS);
                        }
                    } else m_uiMeteorTimer -= uiDiff;

                    if (m_uiFieryTimer <= uiDiff)
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 45.0f, true);
                        if (target)
                        {
                            DoCast(target, SPELL_FIERY_COMBUSTION);
                            m_uiFieryTimer = urand(25*IN_MILLISECONDS,40*IN_MILLISECONDS);
                        }
                    } else m_uiFieryTimer -= uiDiff;

                    break;

                default:
                    break;
            }

            if (m_uiEnrage <= uiDiff)
            {
                DoCast(SPELL_BERSERK);
                m_uiEnrage = 600*IN_MILLISECONDS;
                DoScriptText(-1666105,me);
            } else m_uiEnrage -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};

class boss_halion_twilight : public CreatureScript
{
public:
    boss_halion_twilight() : CreatureScript("boss_halion_twilight") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_halion_twilightAI(pCreature);
    }

    struct boss_halion_twilightAI : public ScriptedAI
    {
        boss_halion_twilightAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        uint8 stage;
        bool intro;
        uint32 m_uiEnrage;
        uint32 m_uiDuskTimer;
        uint32 m_uiDarkBreathTimer;
        uint32 m_uiSoulCunsumTimer;
        uint32 m_uiTailLashTimer;

        void Reset()
        {
            if(!instance)
                return;
            me->SetRespawnDelay(7*DAY);
            setStage(0);
            intro = false;

            m_uiEnrage = 600*IN_MILLISECONDS;
            m_uiDuskTimer = 2*IN_MILLISECONDS;
            m_uiDarkBreathTimer = urand(12*IN_MILLISECONDS,20*IN_MILLISECONDS);
            m_uiSoulCunsumTimer = urand(30*IN_MILLISECONDS,40*IN_MILLISECONDS);
            m_uiTailLashTimer = urand(10*IN_MILLISECONDS,20*IN_MILLISECONDS);
			me->SummonGameObject(GO_FLAME_CREP, 3154.56f, 535.418f, 72.8889f, 4.47206f, 0, 0, 0.786772f, -0.617243f, 0); // circulo crepuscular
            if (GameObject* pGoRing = instance->instance->GetGameObject(instance->GetData64(GO_FLAME_CREP)))
				pGoRing->SetPhaseMask(65535,true);

            me->SetInCombatWithZone();
            if (Creature* pControl = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_CONTROL)))
            {
                me->SetInCombatWith(pControl);
                pControl->SetInCombatWith(me);
            }

            Creature* pFocus = me->GetMap()->GetCreature(instance->GetData64(NPC_ORB_ROTATION_FOCUS));
            if (!pFocus )
                pFocus = me->SummonCreature(NPC_ORB_ROTATION_FOCUS, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 0, TEMPSUMMON_MANUAL_DESPAWN, 1000);
            else if (!pFocus->isAlive())
                pFocus->Respawn();

            if (Creature* pReal = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_REAL)))
                if (pReal->isAlive())
                    me->SetHealth(pReal->GetHealth());
            if (!me->HasAura(SPELL_TWILIGHT_ENTER))
                DoCast(me, SPELL_TWILIGHT_ENTER);
        }

        void setStage(uint8 phase)
        {
            stage = phase;
        }

        uint8 getStage()
        {
            return stage;
        }

        void JustReachedHome()
        {
            if (!instance)
                return;

            if (instance->GetData(TYPE_HALION_EVENT) != FAIL || getStage() == 0)
                return;
			if (GameObject* pGOTemp = me->FindNearestGameObject(GO_FLAME_CREP, 100.0f))
                   pGOTemp->Delete();
        }

        void EnterEvadeMode()
        {

            if (!instance)
                return;

            if (instance->GetData(TYPE_HALION_EVENT) != FAIL || getStage() == 0)
                return;

            ScriptedAI::EnterEvadeMode();
        }

        void MoveInLineOfSight(Unit* pWho)
        {
            if (!instance) return;

            if (!pWho || pWho->GetTypeId() != TYPEID_PLAYER)
                return;

            if ( !intro && pWho->IsWithinDistInMap(me, 20.0f))
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PASSIVE);
                intro = true;
                AttackStart(pWho);
                setStage(1);
                DoCast(SPELL_TWILIGHT_PRECISION);
                if (Creature* pReal = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_REAL)))
                    if (pReal->isAlive())
                       me->SetHealth(pReal->GetHealth());
            }

        }

        void JustDied(Unit* pKiller)
        {
            if (!instance)
                return;
            DoScriptText(-1666104,me);
            instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TWILIGHT_ENTER);

            if (GameObject* pGoPortal = me->FindNearestGameObject(GO_HALION_PORTAL_1, 50.0f))
                   pGoPortal->Delete();
            if (GameObject* pGoPortal = me->FindNearestGameObject(GO_HALION_PORTAL_2, 50.0f))
                   pGoPortal->Delete();
            if (GameObject* pGoPortal = me->FindNearestGameObject(GO_HALION_PORTAL_3, 50.0f))
                   pGoPortal->Delete();
			if (GameObject* pGOTemp = me->FindNearestGameObject(GO_FLAME_CREP, 100.0f))
                   pGOTemp->Delete();

            if (Creature* pReal = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_REAL)))
                if (!pReal->isAlive())
                {
                    instance->SetData(TYPE_HALION, DONE);
                    pReal->SetFlag(UNIT_DYNAMIC_FLAGS, UNIT_DYNFLAG_LOOTABLE);
                    instance->SetData(TYPE_COUNTER, 0);
                }
            me->ForcedDespawn();
        }

        void KilledUnit(Unit* pVictim)
        {
            switch (urand(0,1))
            {
                case 0:
                    DoScriptText(-1631006,me,pVictim);
                    break;
                case 1:
                    DoScriptText(-1631007,me,pVictim);
                    break;
            }
        }

        void EnterCombat(Unit* pWho)
        {
            if (!instance)
                return;
        }

        void UpdateAI(const uint32 uiDiff)
        {

            if (!me->HasAura(SPELL_TWILIGHT_ENTER))
                DoCast(me, SPELL_TWILIGHT_ENTER);

            if (!instance)
            {
                me->ForcedDespawn();
                    return;
            }

            if (!instance || instance->GetData(TYPE_HALION) != IN_PROGRESS || instance->GetData(TYPE_HALION_EVENT) == FAIL)
            {
                if (Creature* pReal = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_REAL)))
                    if (!pReal->isAlive())
                        pReal->Respawn();

                me->ForcedDespawn();
            }

            if (!UpdateVictim())
                return;

            switch (getStage())
            {
                case 1:           //SPAWNED - Twilight realm
//                DoCast(SPELL_TWILIGHT_DIVISION);

                    if (m_uiDuskTimer <= uiDiff)
                    {
                        if (!me->IsNonMeleeSpellCasted(false))
                            DoCast(SPELL_DUSK_SHROUD);
                        m_uiDuskTimer = 2*IN_MILLISECONDS;
                    } else m_uiDuskTimer -= uiDiff;

                    if (m_uiDarkBreathTimer <= uiDiff)
                    {
                        DoCast(SPELL_DARK_BREATH);
                        m_uiDarkBreathTimer = urand(12*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiDarkBreathTimer -= uiDiff;

                    if (m_uiTailLashTimer <= uiDiff)
                    {
                        DoCast(SPELL_TAIL_LASH);
                        m_uiTailLashTimer = urand(10*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiTailLashTimer -= uiDiff;

                    if (m_uiSoulCunsumTimer <= uiDiff)
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 45.0f, true);
                        if (target)
                        {
                            DoCast(target, SPELL_SOUL_CONSUMPTION);
                            m_uiSoulCunsumTimer = urand(25*IN_MILLISECONDS,40*IN_MILLISECONDS);
                        }
                    } else m_uiSoulCunsumTimer -= uiDiff;

                    if ( HealthBelowPct(51) )
                        setStage(2);
                    break;

                case 2:           //To two realms
                    instance->SetData(TYPE_HALION_EVENT, IN_PROGRESS);
                    DoScriptText(-1666109,me);
                    if (GameObject* pGoPortal = me->SummonGameObject(GO_HALION_PORTAL_3, SpawnLoc[0].x, SpawnLoc[0].y, SpawnLoc[0].z, 4.47206f, 0, 0, 0.786772f, -0.617243f, 99999999))
                    {
                        pGoPortal->SetPhaseMask(32,true);
                        pGoPortal->SetRespawnTime(9999999);
                        pGoPortal->SetOwnerGUID(NULL);
                    }
                    DoCast(SPELL_TWILIGHT_DIVISION);
                    setStage(3);
                    break;
                case 3: //PHASE 3 BOTH REALMS
                    if (m_uiDuskTimer <= uiDiff)
                    {
                        if (!me->IsNonMeleeSpellCasted(false))
                            DoCast(SPELL_DUSK_SHROUD);
                        m_uiDuskTimer = 2*IN_MILLISECONDS;
                    } else m_uiDuskTimer -= uiDiff;

                    if (m_uiDarkBreathTimer <= uiDiff)
                    {
                        DoCast(SPELL_DARK_BREATH);
                        m_uiDarkBreathTimer = urand(12*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiDarkBreathTimer -= uiDiff;

                    if (m_uiTailLashTimer <= uiDiff)
                    {
                        DoCast(SPELL_TAIL_LASH);
                        m_uiTailLashTimer = urand(10*IN_MILLISECONDS,20*IN_MILLISECONDS);
                    } else m_uiTailLashTimer -= uiDiff;

                    if (m_uiSoulCunsumTimer <= uiDiff)
                    {
                        Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1, 45.0f, true);
                        if (target)
                        {
                            DoCast(target, SPELL_SOUL_CONSUMPTION);
                            m_uiSoulCunsumTimer = urand(25*IN_MILLISECONDS,40*IN_MILLISECONDS);
                        }
                    } else m_uiSoulCunsumTimer -= uiDiff;
                    break;
                default:
                    break;
            }

            if (m_uiEnrage <= uiDiff)
            {
                 DoCast(SPELL_BERSERK);
                 m_uiEnrage = 600*IN_MILLISECONDS;
                 DoScriptText(-1666105,me);
            } else m_uiEnrage -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};

struct HalionBuffLine
{
    float diff;                // Health diff in percent
    uint32 real, twilight;     // Buff pair
};

static HalionBuffLine Buff[]=
{
    {-10.0f,SPELL_CORPOREALITY_100I, SPELL_CORPOREALITY_100D},
    {-8.0f,SPELL_CORPOREALITY_80I, SPELL_CORPOREALITY_80D},
    {-6.0f,SPELL_CORPOREALITY_60I, SPELL_CORPOREALITY_60D},
    {-4.0f,SPELL_CORPOREALITY_40I, SPELL_CORPOREALITY_40D},
    {-2.0f,SPELL_CORPOREALITY_20I, SPELL_CORPOREALITY_20D},
    {-1.0f,SPELL_CORPOREALITY_EVEN, SPELL_CORPOREALITY_EVEN},
    {1.0f,SPELL_CORPOREALITY_EVEN, SPELL_CORPOREALITY_EVEN},
    {2.0f,SPELL_CORPOREALITY_20D, SPELL_CORPOREALITY_20I},
    {4.0f,SPELL_CORPOREALITY_40D, SPELL_CORPOREALITY_40I},
    {6.0f,SPELL_CORPOREALITY_60D, SPELL_CORPOREALITY_60I},
    {8.0f,SPELL_CORPOREALITY_80D, SPELL_CORPOREALITY_80I},
    {10.0f,SPELL_CORPOREALITY_100D, SPELL_CORPOREALITY_100I},
};

class mob_halion_control : public CreatureScript
{
public:
    mob_halion_control() : CreatureScript("mob_halion_control") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_halion_controlAI(pCreature);
    }

    struct mob_halion_controlAI : public ScriptedAI
    {
        mob_halion_controlAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;
        Creature* pHalionReal;
        Creature* pHalionTwilight;
        uint32 m_lastBuffReal, m_lastBuffTwilight;
        uint32 m_uiCorporealityTimer;
        bool m_detectplayers;

        void Reset()
        {
            if (!instance)
                return;

            m_uiCorporealityTimer = 5*IN_MILLISECONDS;
            m_detectplayers = true;
            me->SetDisplayId(11686);
            me->SetPhaseMask(65535, true);
//        me->SetDisplayId(10045);
            me->SetRespawnDelay(7*DAY);
            SetCombatMovement(false);
            m_lastBuffReal = 0;
            m_lastBuffTwilight = 0;
            instance->SetData(TYPE_COUNTER, 0);
            instance->SetData(TYPE_HALION_EVENT, NOT_STARTED);
        }

        void AttackStart(Unit *who)
        {
            //ignore all attackstart commands
            return;
        }

        bool doSearchPlayerAtRange(float range)
        {
            Map* pMap = me->GetMap();
            if (pMap && pMap->IsDungeon())
            {
                Map::PlayerList const &PlayerList = pMap->GetPlayers();
                if (!PlayerList.isEmpty())
                   for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                    {
                       if (!i->getSource()->IsInMap(me)) continue;
                       if (i->getSource()->isGameMaster()) continue;
                       if (i->getSource()->isAlive()  && i->getSource()->IsWithinDistInMap(me, range))
                           return true;
                    }
            }
            return false;
        }

        void UpdateAI(const uint32 diff)
        {
            if (!instance)
            {
                me->ForcedDespawn();
                return;
            }

            if (instance->GetData(TYPE_HALION) != IN_PROGRESS)
            {
                me->ForcedDespawn();
                return;
            }

            if (m_uiCorporealityTimer <= diff)
            {
                if (!doSearchPlayerAtRange(100.0f))
                {
                    sLog->outDebug(LOG_FILTER_MAPS, "ruby_sanctum: cannot detect players in range! ");
                    if (!m_detectplayers)
                    {
                        instance->SetData(TYPE_HALION_EVENT, FAIL);
                        instance->SetData(TYPE_HALION, FAIL);
                        me->ForcedDespawn();
                    } else m_detectplayers = false;
                } else m_detectplayers = true;

                if (instance->GetData(TYPE_HALION_EVENT) != SPECIAL)
                    return;

                pHalionReal = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_REAL));
                pHalionTwilight = me->GetMap()->GetCreature(instance->GetData64(NPC_HALION_TWILIGHT));

                float p_RealHP = (pHalionReal && pHalionReal->isAlive()) ? pHalionReal->GetHealthPct() : 100.0f;
                float p_TwilightHP = (pHalionTwilight && pHalionTwilight->isAlive()) ? pHalionTwilight->GetHealthPct() : 100.0f;

                float m_diff = (p_RealHP - p_TwilightHP);

                uint8 buffnum;

                if (m_diff <= Buff[0].diff) buffnum = 0;
                else for (uint8 i = 0; i < 11; i++)
                    if (m_diff >= Buff[i].diff)
                        buffnum = i+1;
                    else
                    break;

                if (!m_lastBuffReal || m_lastBuffReal != Buff[buffnum].real)
                {
                    if (pHalionReal && pHalionReal->isAlive())
                    {
                        if (m_lastBuffReal) pHalionReal->RemoveAurasDueToSpell(m_lastBuffReal);
                            pHalionReal->CastSpell(pHalionReal, Buff[buffnum].real, true);
                        m_lastBuffReal = Buff[buffnum].real;
                    }
                }

                if (!m_lastBuffTwilight || m_lastBuffTwilight != Buff[buffnum].twilight)
                {
                    if (pHalionTwilight && pHalionTwilight->isAlive())
                    {
                        if (m_lastBuffTwilight) pHalionTwilight->RemoveAurasDueToSpell(m_lastBuffTwilight);
                            pHalionTwilight->CastSpell(pHalionTwilight, Buff[buffnum].twilight, true);
                        m_lastBuffTwilight = Buff[buffnum].twilight;
                    }
                }

                instance->SetData(TYPE_COUNTER, 50 + (int)Buff[buffnum].diff);
                m_uiCorporealityTimer = 5*IN_MILLISECONDS;
            } else m_uiCorporealityTimer -= diff;
        }
    };
};

class mob_orb_rotation_focus : public CreatureScript
{
public:
    mob_orb_rotation_focus() : CreatureScript("mob_orb_rotation_focus") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_orb_rotation_focusAI(pCreature);
    }

    struct mob_orb_rotation_focusAI : public ScriptedAI
    {
        mob_orb_rotation_focusAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        uint32 m_timer;
        float m_direction, m_nextdirection;
        bool m_warning;

        void Reset()
        {
            me->SetDisplayId(11686);
//        me->SetDisplayId(10045);
            me->SetRespawnDelay(7*DAY);
            me->SetPhaseMask(65535, true);
            SetCombatMovement(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_direction = 0.0f;
            m_nextdirection = 0.0f;
            m_timer = 30000;
            m_warning = false;

            Creature* pPulsar1 = me->GetMap()->GetCreature(instance->GetData64(NPC_SHADOW_PULSAR_N));
            if (!pPulsar1 )
            {
                float x,y;
                me->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
                pPulsar1 = me->SummonCreature(NPC_SHADOW_PULSAR_N, x, y, me->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
            } else if (!pPulsar1->isAlive())
                pPulsar1->Respawn();

            Creature* pPulsar2 = me->GetMap()->GetCreature(instance->GetData64(NPC_SHADOW_PULSAR_S));
            if (!pPulsar2)
            {
                float x,y;
                me->GetNearPoint2D(x, y, FR_RADIUS, m_direction + M_PI);
                pPulsar2 = me->SummonCreature(NPC_SHADOW_PULSAR_S, x, y, me->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
            } else if (!pPulsar2->isAlive())
                pPulsar2->Respawn();
        }

        void AttackStart(Unit *who)
        {
            //ignore all attackstart commands
            return;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!instance)
                  me->ForcedDespawn();
            if (instance->GetData(TYPE_HALION) != IN_PROGRESS)
                  me->ForcedDespawn();

            if (instance->GetData(DATA_ORB_S) == DONE && instance->GetData(DATA_ORB_N) == DONE)
            {
                m_direction = m_nextdirection;
                m_nextdirection = (m_direction - M_PI/64.0f);
                if (m_nextdirection < 0.0f )
                    m_nextdirection = m_nextdirection + 2.0f*M_PI;
                instance->SetData(DATA_ORB_DIRECTION, (uint32)(m_nextdirection*1000));
                instance->SetData(DATA_ORB_N, SPECIAL);
                instance->SetData(DATA_ORB_S, SPECIAL);
                sLog->outDebug(LOG_FILTER_MAPS, "EventMGR: creature %u send direction %u ",me->GetEntry(),instance->GetData(DATA_ORB_DIRECTION));
            }

            if (m_timer - 6000 <= uiDiff && !m_warning)
            {
                DoScriptText(-1666110,me);
                m_warning = true;
            }

            Creature* pPulsar1 = me->GetMap()->GetCreature(instance->GetData64(NPC_SHADOW_PULSAR_N));
            Creature* pPulsar2 = me->GetMap()->GetCreature(instance->GetData64(NPC_SHADOW_PULSAR_S));
            if (m_timer <= uiDiff)
            {
                float x,y;
                me->GetNearPoint2D(x, y, FR_RADIUS, m_nextdirection);
                me->SummonCreature(NPC_ORB_CARRIER, x, y, me->GetPositionZ(), 0, TEMPSUMMON_MANUAL_DESPAWN, 5000);
                m_timer = 30000;
                m_warning = false;
                pPulsar1->CastSpell(pPulsar2, SPELL_TWILIGHT_CUTTER_CHANNEL,false);
                //pPulsar2->CastSpell(pPulsar1, SPELL_TWILIGHT_CUTTER,false);
            } else m_timer -= uiDiff;
        }
    };
};

class mob_halion_orb : public CreatureScript
{
public:
    mob_halion_orb() : CreatureScript("mob_halion_orb") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_halion_orbAI(pCreature);
    }

    struct mob_halion_orbAI : public ScriptedAI
    {
        mob_halion_orbAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        float m_direction,m_delta;
        uint32 m_flag;
        uint32 m_flag1;
        bool MovementStarted;
        Creature* focus;
        uint32 nextPoint;

        void Reset()
        {
            if (!instance)
                return;
            me->SetRespawnDelay(7*DAY);
           // me->SetDisplayId(11686);
            SetCombatMovement(false);
            me->SetPhaseMask(32, true);
            //me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            if (me->GetEntry() == NPC_SHADOW_PULSAR_N)
            {
                m_flag = DATA_ORB_N;
                m_delta = 0.0f;
            } else if (me->GetEntry() == NPC_SHADOW_PULSAR_S)
            {
                m_flag = DATA_ORB_S;
                m_delta = M_PI;
            };
            m_direction = 0.0f;
            nextPoint = 0;
            MovementStarted = false;
            instance->SetData(m_flag, DONE);
            sLog->outDebug(LOG_FILTER_MAPS, "EventMGR: creature %u assume m_flag %u ",me->GetEntry(),m_flag);
        }

        void AttackStart(Unit *who)
        {
            //ignore all attackstart commands
            return;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (!instance)
                return;

            if (type != POINT_MOTION_TYPE || !MovementStarted)
                return;

            if (id == nextPoint)
            {
                me->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                instance->SetData(m_flag, DONE);
            }
        }

        void StartMovement(uint32 id)
        {
            if (!instance)
                return;

            nextPoint = id;
            float x,y;
            instance->SetData(m_flag, IN_PROGRESS);
            MovementStarted = true;
            m_direction = ((float)instance->GetData(DATA_ORB_DIRECTION)/1000 + m_delta);
            if (m_direction > 2.0f*M_PI) m_direction = m_direction - 2.0f*M_PI;
            if (focus = me->GetMap()->GetCreature(instance->GetData64(NPC_ORB_ROTATION_FOCUS)))
                focus->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
            else
                me->ForcedDespawn();
            me->GetMotionMaster()->Clear();
            me->GetMotionMaster()->MovePoint(id, x, y,  me->GetPositionZ());
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!instance)
                  me->ForcedDespawn();
            if (instance->GetData(TYPE_HALION) != IN_PROGRESS)
                  me->ForcedDespawn();
            if (!MovementStarted && instance->GetData(m_flag) == SPECIAL)
                StartMovement(1);
        }
    };
};

class mob_orb_carrier : public CreatureScript
{
public:
    mob_orb_carrier() : CreatureScript("mob_orb_carrier") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_orb_carrierAI(pCreature);
    }

    struct mob_orb_carrierAI : public ScriptedAI
    {
        mob_orb_carrierAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* instance;

        bool MovementStarted;

        void Reset()
        {
//          me->SetDisplayId(10045);
      //    me->SetDisplayId(11686);
            me->SetRespawnDelay(7*DAY);
            SetCombatMovement(false);
            me->SetPhaseMask(32, true);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            MovementStarted = false;
            me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING); //or remove???
            me->SetSpeed(MOVE_RUN, 6.0f, true);
        }

        void AttackStart(Unit *pWho)
        {
            return;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (!instance) return;

            if (type != POINT_MOTION_TYPE || !MovementStarted)
                return;

            if (id == 1)
            {
                me->GetMotionMaster()->MovementExpired();
                MovementStarted = false;
                me->ForcedDespawn();
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!instance || instance->GetData(TYPE_HALION) != IN_PROGRESS)
                  me->ForcedDespawn();
            if (!MovementStarted)
            {
                float x,y;
                float m_direction = ((float)instance->GetData(DATA_ORB_DIRECTION)/1000.0f + M_PI - M_PI/32.0f);
                if (m_direction > 2.0f*M_PI) m_direction = m_direction - 2.0f*M_PI;
                if (Creature* focus = me->GetMap()->GetCreature(instance->GetData64(NPC_ORB_ROTATION_FOCUS)))
                    focus->GetNearPoint2D(x, y, FR_RADIUS, m_direction);
                else me->ForcedDespawn();
                me->GetMotionMaster()->Clear();
                me->GetMotionMaster()->MovePoint(1, x, y,  me->GetPositionZ());
                MovementStarted = true;
            }
        }
    };
};

class mob_soul_consumption : public CreatureScript
{
public:
    mob_soul_consumption() : CreatureScript("mob_soul_consumption") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_soul_consumptionAI(pCreature);
    }

    struct mob_soul_consumptionAI : public ScriptedAI
    {
        mob_soul_consumptionAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* m_instance;

        float m_Size0;
        float m_Size;
        uint32 m_uiConsumptTimer;

        void Reset()
        {
            if (!IsHeroic()) me->SetPhaseMask(32,true);
                else me->SetPhaseMask(65535,true);
            SetCombatMovement(false);
            m_uiConsumptTimer = 60*IN_MILLISECONDS;
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(SPELL_CONSUMPTION_AURA);
            m_Size0 = me->GetFloatValue(OBJECT_FIELD_SCALE_X);
            m_Size = m_Size0;
        }

        void AttackStart(Unit *pWho)
        {
            return;
        }

        void UpdateAI(const uint32 diff)
        {
            if(m_instance && m_instance->GetData(TYPE_HALION) != IN_PROGRESS)
                me->ForcedDespawn();

//        if (!me->HasAura(SPELL_TWILIGHT_ENTER))
//             DoCast(SPELL_TWILIGHT_ENTER);

            if (m_uiConsumptTimer <= diff)
            {
                me->ForcedDespawn();
            }
            else m_uiConsumptTimer -= diff;

            if (SelectTarget(SELECT_TARGET_RANDOM, 1, m_Size * 3.0f, true))
            {
                m_Size = m_Size*1.01f;
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, m_Size);
            }
            else if (SelectTarget(SELECT_TARGET_RANDOM, 0, m_Size * 3.0f, true))
            {
                m_Size = m_Size*1.01f;
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, m_Size);
            }
        }
    };
};

class mob_fiery_combustion : public CreatureScript
{
public:
    mob_fiery_combustion() : CreatureScript("mob_fiery_combustion") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_fiery_combustionAI(pCreature);
    }

    struct mob_fiery_combustionAI : public ScriptedAI
    {
        mob_fiery_combustionAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            m_instance = (InstanceScript*)pCreature->GetInstanceScript();
            Reset();
        }

        InstanceScript* m_instance;

        float m_Size0;
        float m_Size;
        uint32 m_uiConbustTimer;

        void Reset()
        {
            if (!IsHeroic()) me->SetPhaseMask(31,true);
                else me->SetPhaseMask(65535,true);
            m_uiConbustTimer = 60*IN_MILLISECONDS;
            SetCombatMovement(false);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            DoCast(SPELL_COMBUSTION_AURA);
            m_Size0 = me->GetFloatValue(OBJECT_FIELD_SCALE_X);
            m_Size = m_Size0;
        }

        void AttackStart(Unit *pWho)
        {
            return;
        }

        void UpdateAI(const uint32 diff)
        {
            if(m_instance && m_instance->GetData(TYPE_HALION) != IN_PROGRESS)
                me->ForcedDespawn();

            if (m_uiConbustTimer <= diff)
            {
                me->ForcedDespawn();
            }
            else m_uiConbustTimer -= diff;

            if (SelectTarget(SELECT_TARGET_RANDOM, 1, m_Size * 3.0f, true))
            {
                m_Size = m_Size*1.01f;
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, m_Size);
            }
            else if (SelectTarget(SELECT_TARGET_RANDOM, 0, m_Size * 3.0f, true))
            {
                m_Size = m_Size*1.01f;
                me->SetFloatValue(OBJECT_FIELD_SCALE_X, m_Size);
            }
        }

    };
};

#define TARGETS_10 5
#define TARGETS_25 7

class mob_halion_meteor : public CreatureScript
{
public:
    mob_halion_meteor() : CreatureScript("mob_halion_meteor") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_halion_meteorAI(pCreature);
    }

    struct mob_halion_meteorAI : public ScriptedAI
    {
        mob_halion_meteorAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            Reset();
        }

        float x, y, radius, direction;
        uint8 stage;
        uint32 m_uiMeteorImpactTimer;
        uint32 m_uiMeteorStrikeTimer;

        void setStage(uint8 phase)
        {
            stage = phase;
        }

        uint8 getStage()
        {
            return stage;
        }

        void Reset()
        {
            me->SetDisplayId(11686);
            me->SetRespawnDelay(7*DAY);
            SetCombatMovement(false);
            m_uiMeteorImpactTimer = 500;
            m_uiMeteorStrikeTimer = 4500;
            setStage(0);
            me->SetInCombatWithZone();
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        }

        void UpdateAI(const uint32 uiDiff)
        {
            switch (getStage())
            {
                case 0:
                    if (m_uiMeteorImpactTimer <= uiDiff)
                    {
                        DoCast(SPELL_METEOR_IMPACT);
                        m_uiMeteorImpactTimer = 500;
                        setStage(1);
                    } else m_uiMeteorImpactTimer -= uiDiff;
                    break;
                case 1:
                    if (m_uiMeteorStrikeTimer <= uiDiff)
                    {
                        DoCast(SPELL_METEOR_STRIKE);
                        m_uiMeteorStrikeTimer = 4500;
                        setStage(2);
                    } else m_uiMeteorStrikeTimer -= uiDiff;
                    break;
                case 2:
                    // Place summon flames there
                    {
                        direction = 2.0f*M_PI*((float)urand(0,15)/16.0f);
                        radius = 0.0f;
                        for(uint8 i = 0; i < RAID_MODE(TARGETS_10,TARGETS_25,TARGETS_10,TARGETS_25); ++i)
                        {
                            radius = radius + 5.0f;
                            me->GetNearPoint2D(x, y, radius, direction);
                            me->SummonCreature(NPC_METEOR_STRIKE_1, x, y, me->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 20000);
                            me->GetNearPoint2D(x, y, radius, direction+M_PI);
                            me->SummonCreature(NPC_METEOR_STRIKE_1, x, y, me->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 20000);
                        }
                    };

                    {
                        direction = direction + M_PI/4;
                        radius = 0.0f;
                        for(uint8 i = 0; i < RAID_MODE(TARGETS_10,TARGETS_25,TARGETS_10,TARGETS_25); ++i)
                        {
                            radius = radius + 5.0f;
                            me->GetNearPoint2D(x, y, radius, direction);
                            me->SummonCreature(NPC_METEOR_STRIKE_1, x, y, me->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 20000);
                            me->GetNearPoint2D(x, y, radius, direction+M_PI);
                            me->SummonCreature(NPC_METEOR_STRIKE_1, x, y, me->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 20000);
                        }
                     };
                     setStage(3);
                     break;
                case 3:
                    if (m_uiMeteorImpactTimer <= uiDiff)
                    {
                        DoCast(SPELL_METEOR_IMPACT);
                        me->ForcedDespawn();
                        m_uiMeteorImpactTimer = 500;
                    } else m_uiMeteorImpactTimer -= uiDiff;
                    break;
                default:
                     break;
            }
        }
    };
};

class mob_halion_flame : public CreatureScript
{
public:
    mob_halion_flame() : CreatureScript("mob_halion_flame") { }

    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new mob_halion_flameAI(pCreature);
    }

    struct mob_halion_flameAI : public ScriptedAI
    {
        mob_halion_flameAI(Creature* pCreature) : ScriptedAI(pCreature)
        {
            Reset();
        }

        void Reset()
        {
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            me->SetDisplayId(11686);
            me->SetRespawnDelay(7*DAY);
            SetCombatMovement(false);
            me->SetInCombatWithZone();
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!me->HasAura(SPELL_METEOR_FLAME))
                  DoCast(SPELL_METEOR_FLAME);
        }

    };
};

class go_halion_portal_twilight : public GameObjectScript
{
    public:

        go_halion_portal_twilight() : GameObjectScript("go_halion_portal_twilight") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            InstanceScript* instance = (InstanceScript*)go->GetInstanceScript();
            if(!instance)
                return false;
            player->CastSpell(player,SPELL_TWILIGHT_ENTER,false);
                return true;
        }
};

class go_halion_portal_real : public GameObjectScript
{
    public:

        go_halion_portal_real() : GameObjectScript("go_halion_portal_real") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            InstanceScript* instance = (InstanceScript*)go->GetInstanceScript();
            if(!instance)
                return false;
            player->RemoveAurasDueToSpell(SPELL_TWILIGHT_ENTER);
                return true;
        }
};

class spell_halion_fiery_combustion : public SpellScriptLoader
{
    public:
        spell_halion_fiery_combustion() : SpellScriptLoader("spell_halion_fiery_combustion") { }

        class spell_halion_fiery_combustion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halion_fiery_combustion_AuraScript)
            enum Spells
            {
                SPELL_MARK_OF_COMBUSTION  = 74567,
                SPELL_COMBUSTION_EXPLODE  = 74607
            };

            bool Validate(SpellEntry const* /*spellEntry*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_MARK_OF_COMBUSTION))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_COMBUSTION_EXPLODE))
                    return false;
                return true;
            }

            void HandlePeriodicTick(AuraEffect const * /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_MARK_OF_COMBUSTION, true);
            }

            void HandleEffectRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Aura *mark = target->GetAura(SPELL_MARK_OF_COMBUSTION))
                    {
                        int32 bp = 2000 * mark->GetStackAmount();
                        target->CastCustomSpell(target, SPELL_COMBUSTION_EXPLODE, &bp, 0, 0, true);
                        if (Creature* halion_real = GetCaster()->ToCreature())
                        {
                            halion_real->SummonCreature(NPC_COMBUSTION, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        }
                        target->RemoveAura(SPELL_MARK_OF_COMBUSTION, target->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_halion_fiery_combustion_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectRemove += AuraEffectRemoveFn(spell_halion_fiery_combustion_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_fiery_combustion_AuraScript();
        }
};

class spell_halion_soul_consumption : public SpellScriptLoader
{
    public:
        spell_halion_soul_consumption() : SpellScriptLoader("spell_halion_soul_consumption") { }

        class spell_halion_soul_consumption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_halion_soul_consumption_AuraScript)
            enum Spells
            {
                SPELL_MARK_OF_CONSUMPTION  = 74795,
                SPELL_CONSUMPTION_EXPLODE  = 74799
            };

            bool Validate(SpellEntry const* /*spellEntry*/)
            {
                if (!sSpellStore.LookupEntry(SPELL_MARK_OF_CONSUMPTION))
                    return false;
                if (!sSpellStore.LookupEntry(SPELL_CONSUMPTION_EXPLODE))
                    return false;
                return true;
            }

            void HandlePeriodicTick(AuraEffect const * /*aurEff*/)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_MARK_OF_CONSUMPTION, true);
            }

            void HandleEffectRemove(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                {
                    if (Aura *mark = target->GetAura(SPELL_MARK_OF_CONSUMPTION))
                    {
                        int32 bp = 2000 * mark->GetStackAmount();
                        target->CastCustomSpell(target, SPELL_CONSUMPTION_EXPLODE, &bp, 0, 0, true);
                        if (Creature* halion_twilight = GetCaster()->ToCreature())
                        {
                            halion_twilight->SummonCreature(NPC_CONSUMPTION, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ());
                        }
                        target->RemoveAura(SPELL_MARK_OF_CONSUMPTION, target->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_halion_soul_consumption_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
                OnEffectRemove += AuraEffectRemoveFn(spell_halion_soul_consumption_AuraScript::HandleEffectRemove, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_halion_soul_consumption_AuraScript();
        }
};

class spell_twilight_cutter : public SpellScriptLoader
{
    public:
        spell_twilight_cutter() : SpellScriptLoader("spell_twilight_cutter") { }

        class spell_twilight_cutter_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_twilight_cutter_AuraScript)

            void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();

                Unit* target = GetTarget();
                Unit* caster = GetCaster();

                if (!target || !caster)
                    return;

                InstanceScript* instance = GetCaster()->GetInstanceScript();
                Map::PlayerList const &players = instance->instance->GetPlayers();
                for(Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                {
                    if(Player* player = i->getSource())
                    {
                        if (!player) continue;
                        if (player->isGameMaster()) continue;
                        if (!player->HasAura(SPELL_TWILIGHT_ENTER)) continue;
                        if (player->isAlive())
                        {
                            float AB = caster->GetDistance2d(target)+1;
                            float BC = caster->GetDistance2d(player)+1;
                            float AC = target->GetDistance2d(player)+1;
                            float p = (AB + BC + AC)/2;
                            float DC = (2*sqrt(p*(p-AB)*(p-BC)*(p-AC)))/AB;
                            if (DC < 3.75f || DC > 52.0f)
                                target->CastSpell(player, SPELL_TWILIGHT_CUTTER, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_twilight_cutter_AuraScript::HandleTriggerSpell, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_twilight_cutter_AuraScript();
        }
};

// fix ws para dragones
class npc_D_ws2 : public CreatureScript
{
    public:
        npc_D_ws2() : CreatureScript("npc_D_ws2") { }

        struct npc_D_ws2AI : public ScriptedAI
        {
            npc_D_ws2AI(Creature *pCreature) : ScriptedAI(pCreature)
            {
                instance = me->GetInstanceScript();
            }

            uint32 CheckInterval; 

            void InitializeAI()  
            {
                me->SetFlying(true);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);  
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;      

                if (CheckInterval <= diff) 
                {
                    me->SetReactState(REACT_PASSIVE);  
                } 
                else CheckInterval -= diff;

                DoMeleeAttackIfReady();
            }

            private:
                EventMap events;
                InstanceScript* instance;
        };

        CreatureAI* GetAI(Creature *pCreature) const
        {
            return new npc_D_ws2AI(pCreature);
        }
};

void AddSC_boss_halion()
{
    new boss_halion_real();
    new boss_halion_twilight();
    new mob_halion_meteor();
    new mob_halion_flame();
    new mob_halion_orb();
    new mob_halion_control();
    new mob_orb_rotation_focus();
    new mob_orb_carrier();
    new mob_soul_consumption();
    new mob_fiery_combustion();
    new go_halion_portal_twilight();
    new go_halion_portal_real();
    new spell_halion_fiery_combustion();
    new spell_halion_soul_consumption();
    new spell_twilight_cutter();
	new npc_D_ws2();
}
