/*
 * Copyright (c) 2012-2019 scott.cgi All Rights Reserved.
 *
 * This code is licensed under the MIT License:
 * https://github.com/scottcgi/Mojoc/blob/master/LICENSE
 *
 * Since : 2016-6-8
 * Update: 2019-1-8
 * Author: scott.cgi
 */


#ifndef TWEEN_H
#define  TWEEN_H


#include <stddef.h>
#include <stdbool.h>
#include "Engine/Toolkit/Math/TweenEase.h"
#include "Engine/Toolkit/Utils/ArrayList.h"
#include "Engine/Toolkit/HeaderUtils/UserData.h"


typedef float (*TweenActionValueOnGet)(void* target);
typedef void (*TweenActionValueOnSet)(void* target, float value);


typedef struct
{
    /**
     * The action will reach to this value depends on isRelative.
     */
    float                 value;

    /**
     * The action start from this value.
     */
    float                 fromValue;

    /**
     * The final value action will reach to.
     */
    float                 toValue;

    /**
     * When TweenAction's target value need to be get.
     */
    TweenActionValueOnGet OnGet;

    /**
     * When TweenAction's target value need to be set.
     */
    TweenActionValueOnSet OnSet;

    /**
     * The value is relative or absolute, default true.
     */
    bool                  isRelative;

    /**
     * Default TweenEaseType_Linear.
     */
    TweenEaseType         easeType;
}
TweenActionValue;


typedef struct TweenAction TweenAction;
typedef void   (*TweenActionOnComplete)(TweenAction* action);


struct TweenAction
{
    UserData                     userData[1];

    /**
     * Target execute TweenAction.
     */
    void*                        target;

    /**
     * The action running duration time.
     */
    float                        duration;

    /**
     * The action running current position in duration time.
     */
    float                        curTime;

    /**
     * Means action running in queue or immediately, default true.
     */
    bool                         isQueue;

    /**
     * TweenActionValue list changing concurrently.
     */
    ArrayList(TweenActionValue)  actionValueList[1];

    /**
     * When action complete callback.
     */
    TweenActionOnComplete        OnComplete;
};


struct ATween
{
    /**
     * Control by ATween when action completed.
     *
     * userData  : NULL
     * target    : NULL
     * isQueue   : true
     * duration  : 0
     * OnComplete: NULL
     */
    TweenAction*      (*GetAction)            ();

    /**
     * Add TweenActon's TweenActionValue.
     */
    TweenActionValue* (*AddTweenActionValue)  (TweenAction* action);

    /**
     * Bind TweenActions to tweenId and running.
     * if tweenId is NULL will generate tweenId value.
     *
     * return tweenId.
     */
    void*             (*RunActions)           (Array(TweenAction*)* actions, void* tweenId);

    /**
     * Remove tweenId's all actions immediately, return false when tweenId not in use.
     * we can or not cleanup tweenId bound data for actions,
     * but when tweenId not in use then must cleanup for reuse memory
     */
    bool              (*TryRemoveAllActions)  (void* tweenId);

    /**
     * Complete tweenId's all actions immediately, return false when tweenId not in use.
     * if isFireOnComplete true will fire callback.
     */
    bool              (*TryCompleteAllActions)(void* tweenId, bool isFireOnComplete);

    /**
     * Find TweenAction in current or queue, and remove it.
     * if tweenId not in use return false.
     * if not found TweenAction return false.
     */
    bool              (*TryRemoveAction)      (void* tweenId, TweenAction* action);

    /**
     * Whether the tweenId has action in current or queue.
     */
    bool              (*HasAction)            (void* tweenId);

    /**
     * Called every frame by game loop.
     */
    void              (*Update)               (float deltaSeconds);
};


extern struct ATween ATween[1];


#endif
