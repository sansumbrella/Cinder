/*
 Copyright (c) 2011, The Cinder Project, All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Based on the sc-Choreograph CinderBlock by David Wicks: http://sansumbrella.com/

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/Cinder.h"
#include "cinder/TimelineItem.h"
#include "cinder/Easing.h"
#include "cinder/Tween.h"
#include "cinder/Function.h"

#include <boost/intrusive_ptr.hpp>
#include <vector>
#include <list>

namespace cinder {

typedef std::shared_ptr<class Cue>			CueRef;
typedef std::shared_ptr<class Timeline>		TimelineRef;
	
class Timeline : public TimelineItem {		
  public:
	Timeline();			
~Timeline();

	//! Creates a new timeline, defaulted to infinite
	static TimelineRef	create() { TimelineRef result( new Timeline() ); result->setInfinite( true ); return result; }

	//! Advances time a specified amount and evaluates items
	void	step( float timestep );
	//! Goes to a specific time and evaluates items
	void	stepTo( float absoluteTime );
	
	//! Returns the timeline's most recent current time
	float	getCurrentTime() const { return mCurrentTime; }
	
	//! add a cue to the Timeline add the start-time \a atTime
	CueRef add( std::function<void ()> action, float atTime );


	//! Replaces any existing tweens on the \a target with a new tween at the timeline's current time
	template<typename T>
	TweenRef<T> apply( Anim<T> *target, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> )
	{
		target->setParentTimeline( thisRef() );
		return apply( target->ptr(), endValue, duration, easeFunction, lerpFunction );
	}

	//! Creates a new tween and adds it to the end of a timeline, setting its start time to the timeline's duration or the current time, whichever is greater
	template<typename T>
	TweenRef<T> appendBack( Anim<T> *target, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> )
	{
		target->setParentTimeline( thisRef() );
		return appendBack( target->ptr(), endValue, duration, easeFunction, lerpFunction );
	}


	//! Create a new tween and adds it to the timeline's current time
	template<typename T>
	TweenRef<T> add( T *target, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> )
	{
		TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		insert( newTween );
		return newTween;
	}
	
	//! Create a new tween and adds it to the timeline's current time
	template<typename T>
	TweenRef<T> add( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> )
	{
		TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		insert( newTween );
		return newTween;
	}

	//! Replaces any existing tweens on the \a target with a new tween at the timeline's current time
	template<typename T>
	TweenRef<T> apply( T *target, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> )
	{
		TweenRef<T> newTween( new Tween<T>( target, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		apply( newTween );
		return newTween;
	}
	
	//! Replaces any existing tweens on the \a target with a new tween at the timeline's current time
	template<typename T>
	TweenRef<T> apply( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> ) {
		TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		apply( newTween );
		return newTween;
	}

	template<typename T>
	FnTweenRef<T> applyFn( std::function<void (T)> fn, T startValue, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> ) {
		FnTweenRef<T> newTween( new FnTween<T>( fn, startValue, endValue, mCurrentTime, duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		apply( newTween );
		return newTween;
	}

	//! Creates a new tween and adds it to the end of a timeline, setting its start time to the timeline's duration or the current time, whichever is greater
	template<typename T>
	TweenRef<T> appendBack( T *target, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> ) {
		TweenRef<T> newTween( new Tween<T>( target, endValue, std::max( getDuration(), mCurrentTime ), duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		insert( newTween );
		return newTween;
	}
	
	//! Creates a new tween and adds it to the end of a timeline, setting its start time to the timeline's duration or the current time, whichever is greater
	template<typename T>
	TweenRef<T> appendBack( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> ) {
		TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, std::max( getDuration(), mCurrentTime ), duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		insert( newTween );
		return newTween;
	}

	//! Creates a new tween and adds it to the end of the last tween whose target matches \a target. The new tween's start time is set to the previous tween's end time or the current time if no existing tween matches the target, whichever is greater
	template<typename T>
	TweenRef<T> append( T *target, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> ) {
		float startTime = mCurrentTime;
		TimelineItemRef last = findLast( target );
		if( last )
			startTime = last->getEndTime();
		TweenRef<T> newTween( new Tween<T>( target, endValue, std::max( mCurrentTime, startTime ), duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		insert( newTween );
		return newTween;
	}
	
	//! Creates a new tween and adds it to the end of the last tween whose target matches \a target. The new tween's start time is set to the previous tween's end time or the current time, whichever is greater
	template<typename T>
	TweenRef<T> append( T *target, T startValue, T endValue, float duration, EaseFn easeFunction = easeNone, typename Tween<T>::LerpFn lerpFunction = &tweenLerp<T> ) {
		float startTime = mCurrentTime;
		TimelineItemRef last = findLast( target );
		if( last )
			startTime = last->getEndTime();
		TweenRef<T> newTween( new Tween<T>( target, startValue, endValue, std::max( mCurrentTime, startTime ), duration, easeFunction, lerpFunction ) );
		newTween->setAutoRemove( mDefaultAutoRemove );
		insert( newTween );
		return newTween;
	}

	//! Appends to the end of the timeline mirror images of all items
	void appendPingPong();

	//! Replaces any existing TimelineItems that match \a item's target and adds \a item to the timeline. Safe to use from callback fn's.
	void apply( TimelineItemRef item );
	
	//! add an item to the timeline at the current time. Safe to use from callback fn's.
	void add( TimelineItemRef item );
	//! adds an item to the timeline. Its start time is not modified. Safe to use from callback fn's.
	void insert( TimelineItemRef item );
	//! adds an item to the timeline, setting its startTime to be at \a atTime. Safe to use from callback fn's.
	void insert( TimelineItemRef item, float atTime ) { item->mStartTime = atTime; insert( item ); }

	//! Returns the number of items in the Timeline
	size_t				getNumItems() const { return mItems.size(); }
	//! Returns the first item in the timeline the target of which matches \a target
	TimelineItemRef		find( void *target );
	//! Returns the latest-starting item in the timeline the target of which matches \a target
	TimelineItemRef		findLast( void *target );
	//! Removes the TimelineItem \a item from the Timeline. Safe to use from callback fn's.
	void				remove( TimelineItemRef item );
	//! Removes all TimelineItems whose target matches \a target
	void				removeTarget( void *target );
	//! Clones all TimelineItems whose target matches \a target, but replacing their target with \a replacementTarget
	void				cloneAndReplaceTarget( void *target, void *replacementTarget );
	
	//! Remove all tweens from the Timeline. Do not call from callback fn's.
	void clear();
	//! Sets the time to zero, all tweens as not completed, and if \a unsetStarted, marks the tweens as not started. Do not call from callback fn's.
	void reset( bool unsetStarted = false );

	//! Sets the default \a autoRemove value for all future TimelineItems added to the Timeline
	void	setDefaultAutoRemove( bool defaultAutoRemove ) { mDefaultAutoRemove = defaultAutoRemove; }
	//! Returns the default \a autoRemove value for all future TimelineItems added to the Timeline
	bool	getDefaultAutoRemove() const { return mDefaultAutoRemove; }

	//! Call this to notify the Timeline if the \a item's start-time or duration has changed
	void	itemTimeChanged( TimelineItem *item );

	TimelineRef	thisRef() { 
		TimelineItemRef thisTimelineItem = TimelineItem::thisRef();
		TimelineRef result = std::static_pointer_cast<Timeline>( thisTimelineItem );
		return result;
	}
	
  protected:
	virtual void reverse();
	virtual TimelineItemRef cloneReverse() const;
	virtual TimelineItemRef clone() const;
	virtual void start() {}
	virtual void loopStart();
	virtual void update( float absTime );
	virtual void complete() {}

	void						eraseMarked();
	void						calculateDuration();

	bool						mDefaultAutoRemove;
	float						mCurrentTime;
	std::list<TimelineItemRef>	mItems;
};

class Cue : public TimelineItem {
  public:
	Cue( std::function<void ()> fn, float atTime = 0 );

	void					setFn( std::function<void ()> fn ) { mFunction = fn; }
	std::function<void ()>	getFn() const { return mFunction; }
	
  protected:
	virtual void				reverse() { /* no-op */ }
	virtual TimelineItemRef	cloneReverse() const;
	virtual TimelineItemRef clone() const;

	virtual void start() {} // starting is a no-op for Cues
	virtual void loopStart();
	virtual void update( float relativeTime ) {} // update is a no-op for Cues
	virtual void complete() {} // completion is a no-op for Cues
	virtual bool updateAtLoopStart() { return true; }
  
	std::function<void ()>		mFunction;
};

} // namespace cinder