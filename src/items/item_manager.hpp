//
//  SuperTuxKart - a fun racing game with go-kart
//  Copyright (C) 2006-2015 Joerg Henrichs
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 3
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_ITEMMANAGER_HPP
#define HEADER_ITEMMANAGER_HPP

#include "LinearMath/btTransform.h"

#include "items/item.hpp"
#include "utils/aligned_array.hpp"
#include "utils/no_copy.hpp"

#include <SColor.h>

#include <assert.h>
#include <map>
#include <string>
#include <vector>

class Kart;

/**
  * \ingroup items
  */
class ItemManager : public NoCopy
{
    // Some static data and functions to initialise it:
private:
    /** Stores all item models. */
    static std::vector<scene::IMesh *> m_item_mesh;

    /** Stores the glow color for all items. */
    static std::vector<video::SColorf> m_glow_color;

    /** Stores all low-resolution item models. */
    static std::vector<scene::IMesh *> m_item_lowres_mesh;

protected:
    /** The instance of ItemManager while a race is on. */
    static ItemManager *m_item_manager;
public:
    static void loadDefaultItemMeshes();
    static void removeTextures();
    static void create();
    static void destroy();

    // ------------------------------------------------------------------------
    /** Returns the mesh for a certain item. */
    static scene::IMesh* getItemModel(ItemState::ItemType type)
                                      { return m_item_mesh[type]; }
    // ------------------------------------------------------------------------
    /** Returns the glow color for an item. */
    static video::SColorf& getGlowColor(ItemState::ItemType type)
                                      { return m_glow_color[type]; }
    // ------------------------------------------------------------------------
    /** Return an instance of the item manager (it does not automatically
     *  create one, call create for that). */
    static ItemManager *get() {
        assert(m_item_manager);
        return m_item_manager;
    }   // get

    // ========================================================================
protected:
    /** The vector of all items of the current track. */
    typedef std::vector<Item*> AllItemTypes;
    AllItemTypes m_all_items;

private:
    /** Stores which items are on which quad. m_items_in_quads[#quads]
     *  contains all items that are not on a quad. Note that this
     *  field is undefined if no Graph exist, e.g. arena without navmesh. */
    std::vector< AllItemTypes > *m_items_in_quads;

    /** What item this item is switched to. */
    std::vector<ItemState::ItemType> m_switch_to;

    /** Remaining time that items should remain switched. If the
     *  value is <0, it indicates that the items are not switched atm. */
    int m_switch_ticks;

    void  insertItem(Item *item);
    void  deleteItem(Item *item);

public:
    // Make those private so only create/destroy functions can call them.
                   ItemManager();
    virtual       ~ItemManager();
    void           setSwitchItems(const std::vector<int> &switch_items);

public:
    Item*          newItem         (ItemState::ItemType type, const Vec3& xyz,
                                    const Vec3 &normal,
                                    AbstractKart* parent=NULL);
    Item*          newItem         (const Vec3& xyz, float distance,
                                    TriggerItemListener* listener);
    void           update          (int ticks);
    void           updateGraphics  (float dt);
    void           checkItemHit    (AbstractKart* kart);
    void           reset           ();
    virtual void   collectedItem   (Item *item, AbstractKart *kart,
                                    int add_info=-1);
    void           switchItems     ();
    bool           randomItemsForArena(const AlignedArray<btTransform>& pos);
    // ------------------------------------------------------------------------
    /** Only used in the NetworkItemManager. */
    virtual void setItemConfirmationTime(int host_id, int ticks)
    {
        assert(false);
    }
    // ------------------------------------------------------------------------
    /** Returns the number of items. */
    unsigned int   getNumberOfItems() const
    {
        return (unsigned int) m_all_items.size();
    }
    // ------------------------------------------------------------------------
    /** Returns a pointer to the n-th item. */
    const Item*   getItem(unsigned int n) const { return m_all_items[n]; };
    // ------------------------------------------------------------------------
    /** Returns a pointer to the n-th item. */
    Item* getItem(unsigned int n)  { return m_all_items[n]; };
    // ------------------------------------------------------------------------
    /** Returns a reference to the array of all items on the specified quad.
     */
    const AllItemTypes& getItemsInQuads(unsigned int n) const
    {
        assert(m_items_in_quads);
        assert(n<(*m_items_in_quads).size());
        return (*m_items_in_quads)[n];
    }   // getItemsInQuads
    // ------------------------------------------------------------------------
    /** Returns the first item (NULL if none) on the specified quad
     */
    Item* getFirstItemInQuad(unsigned int n) const
    {
        assert(m_items_in_quads);
        assert(n < m_items_in_quads->size());
        return ((*m_items_in_quads)[n]).empty() ? NULL :
            (*m_items_in_quads)[n].front();
    }   // getFirstItemInQuad
};   // ItemManager

#endif
