/*
 * Copyright (C) 2015 Glyptodon LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef GUAC_TERMINAL_SCROLLBAR_H
#define GUAC_TERMINAL_SCROLLBAR_H

#include "config.h"

#include <guacamole/client.h>
#include <guacamole/layer.h>

/**
 * The width of the scrollbar, in pixels.
 */
#define GUAC_TERMINAL_SCROLLBAR_WIDTH 16 

/**
 * The number of pixels between the draggable handle of the scrollbar and the
 * boundary of the containing layer.
 */
#define GUAC_TERMINAL_SCROLLBAR_PADDING 2

/**
 * The minimum height of the draggable handle of the scrollbar, in pixels.
 */
#define GUAC_TERMINAL_SCROLLBAR_MIN_HEIGHT 64

/**
 * The state of all scrollbar components, describing all variable aspects of
 * the scrollbar's appearance.
 */
typedef struct guac_terminal_scrollbar_render_state {

    /**
     * The current X-coordinate of the upper-left corner of the scrollbar's
     * handle. This value will be relative to the scrollbar's containing layer.
     */
    int handle_x;

    /**
     * The current Y-coordinate of the upper-left corner of the scrollbar's
     * handle. This value will be relative to the scrollbar's containing layer.
     */
    int handle_y;

    /**
     * The width of the scrollbar's handle.
     */
    int handle_width;

    /**
     * The height of the scrollbar's handle.
     */
    int handle_height;

    /**
     * The current X-coordinate of the upper-left corner of the scrollbar's
     * containing layer.
     */
    int container_x;

    /**
     * The current Y-coordinate of the upper-left corner of the scrollbar's
     * containing layer.
     */
    int container_y;

    /**
     * The width of the scrollbar's containing layer.
     */
    int container_width;

    /**
     * The height of the scrollbar's containing layer.
     */
    int container_height;

} guac_terminal_scrollbar_render_state;

/**
 * A scrollbar, made up of a containing layer and inner draggable handle. The
 * position of the handle within the layer represents the value of the
 * scrollbar.
 */
typedef struct guac_terminal_scrollbar {

    /**
     * The client associated with this scrollbar.
     */
    guac_client* client;

    /**
     * The layer containing the scrollbar.
     */
    const guac_layer* parent;

    /**
     * The width of the parent layer, in pixels.
     */
    int parent_width;

    /**
     * The height of the parent layer, in pixels.
     */
    int parent_height;

    /**
     * The scrollbar itself.
     */
    guac_layer* container;

    /**
     * The draggable handle within the scrollbar, representing the current
     * scroll value.
     */
    guac_layer* handle;

    /**
     * The minimum scroll value.
     */
    int min;

    /**
     * The maximum scroll value.
     */
    int max;

    /**
     * The size of the visible area, in the same units as min and max.
     */
    int visible_area;

    /**
     * The current scroll value.
     */
    int value;

    /**
     * The current state of all variable, visible parts of the scrollbar.
     */
    guac_terminal_scrollbar_render_state render_state;

} guac_terminal_scrollbar;

/**
 * Allocates a new scrollbar, associating that scrollbar with the given client
 * and parent layer. The dimensions of the parent layer dictate the initial
 * position of the scrollbar. Currently, the scrollbar is always anchored to
 * the right edge of the parent layer.
 *
 * This will cause instructions to be written to the client's socket, but the
 * client's socket will not be automatically flushed.
 *
 * @param client
 *     The client to associate with the new scrollbar.
 *
 * @param parent
 *     The layer which will contain the newly-allocated scrollbar.
 *
 * @param parent_width
 *     The width of the parent layer, in pixels.
 *
 * @param parent_height
 *     The height of the parent layer, in pixels.
 *
 * @param visible_area
 *     The amount of scrollable data that can be shown within the parent layer
 *     at any given time. This value uses the same units as min, max, and the
 *     current scroll value.
 *
 * @return
 *     A newly allocated scrollbar.
 */
guac_terminal_scrollbar* guac_terminal_scrollbar_alloc(guac_client* client,
        const guac_layer* parent, int parent_width, int parent_height,
        int visible_area);

/**
 * Frees the given scrollbar.
 *
 * @param scrollbar
 *     The scrollbar to free.
 */
void guac_terminal_scrollbar_free(guac_terminal_scrollbar* scrollbar);

/**
 * Flushes the render state of the given scrollbar, updating the remote display
 * accordingly.
 *
 * This may cause instructions to be written to the client's socket, but the
 * client's socket will not be automatically flushed.
 *
 * @param scrollbar
 *     The scrollbar whose render state is to be flushed.
 */
void guac_terminal_scrollbar_flush(guac_terminal_scrollbar* scrollbar);

/**
 * Sets the minimum and maximum allowed scroll values of the given scrollbar
 * to the given values. If necessary, the current value of the scrollbar will
 * be adjusted to fit within the new bounds.
 *
 * This may cause instructions to be written to the client's socket, but the
 * client's socket will not be automatically flushed.
 *
 * @param scrollbar
 *     The scrollbar whose bounds are changing.
 *
 * @param min
 *     The new minimum value of the scrollbar.
 *
 * @param max
 *     The new maximum value of the scrollbar.
 */
void guac_terminal_scrollbar_set_bounds(guac_terminal_scrollbar* scrollbar,
        int min, int max);

/**
 * Sets the current value of the given scrollbar. If the value specified does
 * not fall within the scrollbar's defined minimum and maximum values, the
 * value will be adjusted to fit.
 *
 * This may cause instructions to be written to the client's socket, but the
 * client's socket will not be automatically flushed.
 *
 * @param scrollbar
 *     The scrollbar whose value is changing.
 *
 * @param value
 *     The value to assign to the scrollbar. If the value if out of bounds, it
 *     will be automatically adjusted to fit.
 */
void guac_terminal_scrollbar_set_value(guac_terminal_scrollbar* scrollbar,
        int value);

/**
 * Notifies the scrollbar that the parent layer has been resized, and that the
 * scrollbar may need to be repositioned or resized accordingly.
 *
 * This may cause instructions to be written to the client's socket, but the
 * client's socket will not be automatically flushed.
 *
 * @param scrollbar
 *     The scrollbar whose parent layer has been resized.
 *
 * @param parent_width
 *     The new width of the parent layer, in pixels.
 *
 * @param parent_height
 *     The new height of the parent layer, in pixels.
 *
 * @param visible_area
 *     The amount of scrollable data that can be shown within the parent layer
 *     at any given time. This value uses the same units as min, max, and the
 *     current scroll value.
 */
void guac_terminal_scrollbar_parent_resized(guac_terminal_scrollbar* scrollbar,
        int parent_width, int parent_height, int visible_area);

#endif
