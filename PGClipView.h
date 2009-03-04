/* Copyright © 2007-2008, The Sequential Project
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the the Sequential Project nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE SEQUENTIAL PROJECT ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE SEQUENTIAL PROJECT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
#import <Cocoa/Cocoa.h>

// Other
#import "PGGeometryTypes.h"

extern NSString *const PGClipViewBoundsDidChangeNotification;

enum {
	PGScrollByLine = 0,
	PGScrollByPage = 1
};
typedef unsigned PGScrollType;

enum {
	PGNoAnimation = 0,
	PGAllowAnimation = 1,
	PGPreferAnimation = 2
};
typedef unsigned PGAnimationType;

enum {
	PGScrollLeastToRect = 0,
	PGScrollCenterToRect = 1,
	PGScrollMostToRect = 2
};
typedef unsigned PGScrollToRectType;

@interface PGClipView : NSView
{
	@private
	IBOutlet id delegate;
	IBOutlet NSView *documentView;
	NSRect _documentFrame;
	PGInset _boundsInset;
	NSColor *_backgroundColor;
	BOOL _backgroundIsComplex;
	BOOL _showsBorder;
	NSCursor *_cursor;
	NSPoint _immediatePosition;
	NSPoint _position;
	NSTimer *_scrollTimer;
	NSTimeInterval _lastScrollTime;
	PGRectEdgeMask _pinLocation;
	unsigned _documentViewIsResizing;
	BOOL _firstMouse;
	unsigned _scrollCount;
}

- (id)delegate;
- (void)setDelegate:(id)anObject;

- (NSView *)documentView;
- (void)setDocumentView:(NSView *)aView;
- (NSRect)documentFrame;
- (PGInset)boundsInset;
- (void)setBoundsInset:(PGInset)inset;
- (NSRect)insetBounds;

- (NSColor *)backgroundColor;
- (void)setBackgroundColor:(NSColor *)aColor;
- (BOOL)showsBorder;
- (void)setShowsBorder:(BOOL)flag;
- (NSCursor *)cursor;
- (void)setCursor:(NSCursor *)cursor;

- (NSRect)scrollableRectWithBorder:(BOOL)flag;
- (NSSize)distanceInDirection:(PGRectEdgeMask)direction forScrollType:(PGScrollType)scrollType;
- (NSSize)distanceInDirection:(PGRectEdgeMask)direction forScrollType:(PGScrollType)scrollType fromPosition:(NSPoint)position;
- (NSSize)maximumDistanceForScrollType:(PGScrollType)scrollType;
- (BOOL)shouldExitForMovementInDirection:(PGRectEdgeMask)mask;

- (NSPoint)position;
- (NSPoint)positionForScrollAnimation:(PGAnimationType)type;
- (BOOL)scrollTo:(NSPoint)aPoint animation:(PGAnimationType)type;
- (BOOL)scrollBy:(NSSize)aSize animation:(PGAnimationType)type;
- (BOOL)scrollToEdge:(PGRectEdgeMask)mask animation:(PGAnimationType)type;
- (BOOL)scrollToLocation:(PGPageLocation)location animation:(PGAnimationType)type;
- (void)stopAnimatedScrolling;

- (PGRectEdgeMask)pinLocation;
- (void)setPinLocation:(PGRectEdgeMask)mask;
- (NSSize)pinLocationOffset;
- (BOOL)scrollPinLocationToOffset:(NSSize)aSize;

- (NSPoint)center;
- (BOOL)scrollCenterTo:(NSPoint)aPoint animation:(PGAnimationType)type;

- (BOOL)handleMouseDown:(NSEvent *)firstEvent;
- (void)arrowKeyDown:(NSEvent *)firstEvent;
- (void)scrollInDirection:(PGRectEdgeMask)direction type:(PGScrollType)scrollType;
- (void)magicPanForward:(BOOL)forward acrossFirst:(BOOL)across;

- (void)beginScrolling;
- (void)endScrolling;

- (void)viewFrameDidChange:(NSNotification *)aNotif;

@end

@interface NSObject (PGClipViewDelegate)

- (BOOL)clipView:(PGClipView *)sender handleMouseEvent:(NSEvent *)anEvent first:(BOOL)flag;
- (BOOL)clipView:(PGClipView *)sender handleKeyDown:(NSEvent *)anEvent;
- (BOOL)clipView:(PGClipView *)sender shouldExitEdges:(PGRectEdgeMask)mask;
- (PGRectEdgeMask)clipView:(PGClipView *)sender directionFor:(PGPageLocation)pageLocation; // Don't provide contradictory directions.
- (void)clipView:(PGClipView *)sender magnifyBy:(float)amount;
- (void)clipView:(PGClipView *)sender rotateByDegrees:(float)amount;
- (void)clipViewGestureDidEnd:(PGClipView *)sender;

@end

@interface NSView (PGClipViewAdditions)

- (PGClipView *)PG_enclosingClipView;
- (PGClipView *)PG_clipView;

- (void)PG_scrollRectToVisible:(NSRect)aRect type:(PGScrollToRectType)type;
- (void)PG_scrollRectToVisible:(NSRect)aRect forView:(NSView *)view type:(PGScrollToRectType)type;

- (BOOL)PG_acceptsClicksInClipView:(PGClipView *)sender;
- (BOOL)PG_scalesContentWithFrameSizeInClipView:(PGClipView *)sender;
- (void)PG_viewWillScrollInClipView:(PGClipView *)clipView;
- (void)PG_viewDidScrollInClipView:(PGClipView *)clipView;

@end
