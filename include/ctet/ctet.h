#ifndef CTET_H
#define CTET_H

/*
MIT License

Copyright (c) 2023 Dr Ocelot

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/



#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include "stdint.h"
#include "stdbool.h"
#endif

/**********************
   _____ _______   _   
  / ____|__   __| | |  
 | |       | | ___| |_ 
 | |       | |/ _ \ __|
 | |____   | |  __/ |_ 
  \_____|  |_|\___|\__|
  
**********************/

// ===========
// Math
// ===========

/**
 * Represents a 2D point with integer coordinates.
 */
typedef struct {
    int x, y;
} CTetPoint;

/**
 * Add two CTetPoint instances and return a new CTetPoint as the result.
 *
 * @param a The first CTetPoint to be added.
 * @param b The second CTetPoint to be added.
 * @return A new CTetPoint representing the result of the addition.
 */
CTetPoint ctPoint_addToNew(CTetPoint a, CTetPoint b);

/**
 * Add a CTetPoint to another CTetPoint and store the result in the destination CTetPoint.
 *
 * @param dest   A pointer to the destination CTetPoint where the result will be stored.
 * @param addend The CTetPoint to be added to the destination CTetPoint.
 */
void ctPoint_add(CTetPoint *dest, CTetPoint addend);

/**
 * Check if two CTetPoint instances are equal.
 *
 * @param a The first CTetPoint to be compared.
 * @param b The second CTetPoint to be compared.
 * @return true if the two CTetPoint instances are equal (have the same x and y coordinates), false otherwise.
 */
bool ctPoint_isEqual(CTetPoint a, CTetPoint b);

// ===========
// Blocks
// ===========

// Enumeration representing block colors.
typedef enum {
    // Invalid color.
    CTetBlockColor_INVALID = -1,
    // No color (block does not exist).
    CTetBlockColor_NONE,
    // Red color.
    CTetBlockColor_RED,
    // Orange color.
    CTetBlockColor_ORANGE,
    // Yellow color.
    CTetBlockColor_YELLOW,
    // Green color.
    CTetBlockColor_GREEN,
    // Light blue color.
    CTetBlockColor_LIGHT_BLUE,
    // Blue color.
    CTetBlockColor_BLUE,
    // Purple color.
    CTetBlockColor_PURPLE,
} CTetBlockColor;

/**
 * Represents a block in the game.
 */
typedef struct {
    // The color of the block
    CTetBlockColor color;
    // The timestamp at which the block was locked into place on the game field.
    double lockedTimestamp;
} CTetBlock;

#define BLOCK_TIMESTAMP_NOT_LOCKED -1

// ===========
// Pieces
// ===========

// Enumeration representing piece types.
typedef enum {
    // No piece.
    CTetPieceType_NONE = -1,
    // I piece.
    CTetPieceType_I,
    // J piece.
    CTetPieceType_J,
    // L piece.
    CTetPieceType_L,
    // O piece.
    CTetPieceType_O,
    // S piece.
    CTetPieceType_S,
    // T piece.
    CTetPieceType_T,
    // Z piece.
    CTetPieceType_Z,
    // Count of piece types.
    CTetPieceType_LENGTH,
} CTetPieceType;

/**
 * The number of blocks in a single piece.
 */
#define CT_BLOCKS_PER_PIECE 4

/**
 * Enumerates the possible orientations of a piece.
 */
typedef enum {
    // Up (north) orientation.
    CTetOrientation_UP,
    // Right (east) orientation.
    CTetOrientation_RIGHT,
    // Down (south) orientation.
    CTetOrientation_DOWN,
    // Left (west) orientation.
    CTetOrientation_LEFT,
    // Count of orientations.
    Orientation_LENGTH,
} CTetOrientation;

/**
 * Represents a piece.
 */
typedef struct {
    // The blocks that make up the piece.
    CTetBlock blocks[CT_BLOCKS_PER_PIECE];
    // The coordinates of the piece's blocks.
    const CTetPoint *coords;
    // The orientation of the piece.
    CTetOrientation orientation;
    // The type of the piece.
    CTetPieceType type;
} CTetPiece;

// ===========
// Field
// ===========

/**
 * The width of the playing field in the game, measured in blocks.
 */
#define CT_FIELD_WIDTH 10

/**
 * The total height of the playing field in the game, measured in blocks.
 * This includes both the visible play area and any off-screen buffer area.
 */
#define CT_TOTAL_FIELD_HEIGHT 40

/**
 * The height of the visible playing field in the game, measured in blocks.
 * This represents the portion of the field that is normally visible to the player.
 */
#define CT_VISIBLE_FIELD_HEIGHT 20

// ===========
// Next
// ===========

/**
 * The maximum length of the next block queue in the game.
 */
#define CT_NEXT_QUEUE_MAX_LENGTH 5

// ===========
// Engine
// ===========

// A CTet game engine.
typedef struct CTetEngine CTetEngine;

typedef int CTetMessageId;

typedef struct {
    CTetMessageId id;
    int32_t detailA;
    int32_t detailB;
} CTetMessage;

typedef struct {
    int lines;
    int score;
} CTetStats;

/**
 * @brief Initialize a CTetEngine.
 * 
 * This function initializes a CTetEngine by setting up its various components and resetting them to their initial states.
 * 
 * @param engine A pointer to a CTetEngine object.
 */
void ctEngine_init(CTetEngine *engine); 

/**
 * @brief Get the size of the CTetEngine struct.
 * 
 * This function returns the size of the CTetEngine struct in bytes.
 * The CTetEngine struct is used in the CTet game engine.
 * 
 * @return The size of the CTetEngine struct in bytes.
 *
 */
size_t ctEngine_getSize();

/**
 * Reset the CTetEngine to its initial state.
 *
 * @param engine A pointer to the CTetEngine instance to be reset.
 */
void ctEngine_reset(CTetEngine *engine);

// -- Engine Input --

/**
 * Update the CTetEngine's state based on the elapsed time in milliseconds.
 *
 * @param engine      A pointer to the CTetEngine instance to be updated.
 * @param deltaMillis The elapsed time in milliseconds since the last update.
 */
void ctEngine_update(CTetEngine *engine, float deltaMillis);

/**
 * Handle the event when the right shift button is pressed down.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onShiftRightDown(CTetEngine *engine);

/**
 * Handle the event when the right shift button is released.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onShiftRightUp(CTetEngine *engine);

/**
 * Handle the event when the left shift button is pressed down.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onShiftLeftDown(CTetEngine *engine);

/**
 * Handle the event when the left shift button is released.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onShiftLeftUp(CTetEngine *engine);

/**
 * Handle the event when a hard drop action is performed.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onHardDrop(CTetEngine *engine);

/**
 * Handle the event when a left rotation is requested.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onRotateLeft(CTetEngine *engine);

/**
 * Handle the event when a right rotation is requested.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onRotateRight(CTetEngine *engine);

/**
 * Handle the event when the soft drop button is pressed down.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onSoftDropDown(CTetEngine *engine);

/**
 * Handle the event when the soft drop button is released.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onSoftDropUp(CTetEngine *engine);

/**
 * Handle the event when the hold button is pressed down.
 *
 * @param engine A pointer to the CTetEngine instance.
 */
void ctEngine_onHoldDown(CTetEngine *engine);

// -- Engine state info --

/**
 * Get the position (coordinates) of the active piece on the game field.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return A CTetPoint representing the position of the active piece.
 */
CTetPoint ctEngine_getActivePiecePos(const CTetEngine *engine);

/**
 * Get a pointer to the active piece currently in play.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return A pointer to the active CTetPiece.
 */
const CTetPiece *ctEngine_getActivePiece(const CTetEngine *engine);

/**
 * Get the offset (distance) of the ghost piece from the active piece's position.
 * The ghost piece represents where the active piece would land if dropped instantly.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return A CTetPoint representing the offset of the ghost piece.
 */
CTetPoint ctEngine_getGhostOffset(const CTetEngine *engine);

/**
 * Get the remaining lock delay percentage for the active piece.
 * This indicates the time remaining for the active piece to be locked in place.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return The remaining lock delay as a percentage (0 to 1).
 */
float ctEngine_getLockDelayRemainingPercentage(const CTetEngine *engine);

/**
 * Get an array of pointers to the next pieces in the queue.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return A pointer to an array of CTetPiece pointers representing the next pieces.
 */
const CTetPiece *ctEngine_getNextPieces(const CTetEngine *engine);

/**
 * Get the piece currently held by the player.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return A pointer to the held CTetPiece.
 */
const CTetPiece *ctEngine_getHeldPiece(const CTetEngine *engine);

/**
 * Check if the HOLD queue is locked in the CTetEngine.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return True if the HOLD queue is locked, false otherwise.
 */
bool ctEngine_holdIsLocked(const CTetEngine *engine);

/**
 * Get a pointer to the block at a specified location on the game field.
 *
 * @param engine   A pointer to the CTetEngine instance.
 * @param location The coordinates of the field location.
 * @return A pointer to the CTetBlock at the specified location.
 */
const CTetBlock *ctEngine_getBlockAtFieldLocation(const CTetEngine *engine, CTetPoint location);

/**
 * Retrieves the current timestamp within the game's internal clock,
 * representing the amount of time elapsed since the start of the game.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return The current timestamp as a double, representing time in millis.
 */
double ctEngine_getTimestamp(const CTetEngine *engine);

/**
 * Retrieves a pointer to the stats data of the CTetEngine instance
 * 
 * @param engine A pointer to the CTetEngine instance.
 * @return A pointer to the stats.
 */
const CTetStats *ctEngine_getStats(const CTetEngine *engine);

// ===========
// Messages
// ===========

/**
 * Poll the CTetEngine for messages that indicate game events,
 * state changes, or other significant occurrences during gameplay. Each message is represented
 * After a message is retrieved, it is removed from the queue. If there are no more messages,
 * message id CT_MSG_NONE is returned.
 *
 * @param engine A pointer to the CTetEngine instance.
 * @return A CTetMessage structure representing the next message in the queue. If there are no messages,
 *         a message with id CT_MSG_NONE is returned.
 */
CTetMessage ctEngine_nextMessage(CTetEngine *engine);

/**
 * There are no more messages.
 */
#define CT_MSG_NONE 0x0

/**
 * The game is over. DetailA is the type. DetailB is unused.
 */
#define CT_MSG_GAME_OVER 0x1

/**
 * The player died because the next piece could not spawn without an obstruction.
 */
#define CT_GAME_OVER_TYPE_BLOCK_OUT 0x0

/*******************
End
*******************/

#ifdef __cplusplus
}
#endif
#endif
