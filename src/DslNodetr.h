/*
The MIT License

Copyright (c) 2019-2021, Prominence AI, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in-
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef _DSL_NODETR_H
#define _DSL_NODETR_H

#include "Dsl.h"
#include "DslApi.h"
#include "DslBase.h"

namespace DSL
{

    /**
     * @brief convenience macros for shared pointer abstraction
     */
    #define DSL_NODETR_PTR std::shared_ptr<Nodetr>
    #define DSL_NODETR_NEW(name) \
        std::shared_ptr<Nodetr>(new Nodetr(name))    

    #define DSL_GSTNODETR_PTR std::shared_ptr<GstNodetr>
    #define DSL_GSTNODETR_NEW(name) \
        std::shared_ptr<GstNodetr>(new GstNodetr(name))    

    /**
     * @class Nodetr
     * @brief Implements a container class for all DSL Tree Node types
     */
    class Nodetr : public Base
    {
    public:
        
        /**
         * @brief ctor for the Nodetr base class
         * @param[in] name for the new Nodetr
         */
        Nodetr(const char* name)
            : Base(name)
            , m_pGstObj(NULL)
            , m_pParentGstObj(NULL)
        {
            LOG_FUNC();

            LOG_DEBUG("New Nodetr '" << m_name << "' created");
        }

        /**
         * @brief dtor for the Nodetr base class
         */
        ~Nodetr()
        {
            LOG_FUNC();
            
            LOG_DEBUG("Nodetr '" << m_name << "' deleted");
        }
        
        /**
         * @brief Links this Noder, becoming a source, to a sink Nodre
         * @param[in] pSink Sink Nodre to link this Source Nodre to
         * @return True if this source Nodetr is successfully linked to the provided sink Nodetr
         */
        virtual bool LinkToSink(DSL_NODETR_PTR pSink)
        {
            LOG_FUNC();

            if (m_pSink)
            {
                LOG_ERROR("Nodetr '" << GetName() << "' is currently in a linked to Sink");
                return false;
            }
            m_pSink = pSink;
            LOG_DEBUG("Source '" << GetName() << "' linked to Sink '" << pSink->GetName() << "'");
            
            return true;
        }
        
        /**
         * @brief Unlinks this Source Nodetr from its Sink Nodetr
         * @return True if this source Nodetr is successfully unlinked the sink Nodetr
         */
        virtual bool UnlinkFromSink()
        {
            LOG_FUNC();

            if (!m_pSink)
            {
                LOG_ERROR("Nodetr '" << GetName() << "' is not currently linked to Sink");
                return false;
            }
            LOG_DEBUG("Unlinking Source '" << GetName() <<"' from Sink '" << m_pSink->GetName() << "'");
            m_pSink = nullptr; 

            return true;
        }
        
        /**
         * @brief Links this Noder, becoming a source, to a sink Nodre
         * @param[in] pSrc Nodre to link this Sink Nodre back to
         */
        virtual bool LinkToSource(DSL_NODETR_PTR pSrc)
        {
            LOG_FUNC();

            if (m_pSrc)
            {
                LOG_ERROR("Nodetr '" << GetName() << "' is currently in a linked to a Source");
                return false;
            }
            m_pSrc = pSrc;
            LOG_DEBUG("Source '" << pSrc->GetName() << "' linked to Sink '" << GetName() << "'");
            
            return true;
        }

        /**
         * @brief returns the currently linked to Sink Nodetr
         * @return shared pointer to Sink Nodetr, nullptr if Unlinked to sink
         */
        DSL_NODETR_PTR GetSource()
        {
            LOG_FUNC();
            
            return m_pSrc;
        }

        /**
         * @brief Unlinks this Sink Nodetr from its Source Nodetr
         */
        virtual bool UnlinkFromSource()
        {
            LOG_FUNC();

            if (!m_pSrc)
            {
                LOG_ERROR("Nodetr '" << GetName() << "' is not currently linked to Source");
                return false;
            }
            LOG_DEBUG("Unlinking self '" << GetName() <<"' as a Sink from '" << m_pSrc->GetName() << "' Source");
            m_pSrc = nullptr;
            
            return true;
        }
        
        /**
         * @brief returns the Source-to-Sink linked state for this Nodetr
         * @return true if this Nodetr is linked to a Sink Nodetr
         */
        bool IsLinkedToSink()
        {
            LOG_FUNC();
            
            return bool(m_pSink);
        }
        
        /**
         * @brief returns the Sink-to-Source linked state for this Nodetr
         * @return true if this Nodetr is linked to a Source Nodetr
         */
        bool IsLinkedToSource()
        {
            LOG_FUNC();
            
            return bool(m_pSrc);
        }
        
        /**
         * @brief returns the currently linked to Sink Nodetr
         * @return shared pointer to Sink Nodetr, nullptr if Unlinked to sink
         */
        DSL_NODETR_PTR GetSink()
        {
            LOG_FUNC();
            
            return m_pSink;
        }
        
        GstObject* GetGstObject()
        {
            LOG_FUNC();
            
            return GST_OBJECT(m_pGstObj);
        }
        
        GstElement* GetGstElement()
        {
            LOG_FUNC();
            
            return GST_ELEMENT(m_pGstObj);
        }
        
        GObject* GetGObject()
        {
            LOG_FUNC();
            
            return G_OBJECT(m_pGstObj);
        }

        GstObject* GetParentGstObject()
        {
            LOG_FUNC();
            
            return GST_OBJECT(m_pParentGstObj);
        }

        GstElement* GetParentGstElement()
        {
            LOG_FUNC();
            
            return GST_ELEMENT(m_pParentGstObj);
        }
        
        void SetGstObject(GstObject* pGstObj)
        {
            LOG_FUNC();
            
            m_pGstObj = pGstObj;
        }

    public:
    
        /**
         * @brief Parent of this Nodetr if one exists. NULL otherwise
         */
        GstObject* m_pParentGstObj;
        
        
    protected:

        /**
         * @brief Gst object wrapped by the Nodetr
         */
        GstObject* m_pGstObj;

        /**
         * @brief defines the relationship between a Source Nodetr
         * linked to this Nodetr, making this Nodetr a Sink
         */
        DSL_NODETR_PTR m_pSrc;

        /**
         * @brief defines the relationship this Nodetr linked to
         * a Sink Nodetr making this Nodetr a Source
         */
        DSL_NODETR_PTR m_pSink;
    };

    static GstPadProbeReturn complete_unlink_from_source_tee_cb(GstPad* pad, 
        GstPadProbeInfo *info, gpointer pNoder);

   /**
     * @class GstNodetr
     * @brief Overrides the Base Class Virtual functions, adding the actuall GstObject* management
     * This allows the Nodetr class, and all its relational behavior, to be tested independent from GStreamer
     * Each method of this class calls the base class to complete its behavior.
     */
    class GstNodetr : public Nodetr
    {
    public:
        
        /**
         * @brief ctor for the GstNodetr base class
         * @param[in] name for the new GstNodetr
         */
        GstNodetr(const char* name)
            : Nodetr(name)
            , m_pGstStaticNodetrSinkPad(NULL)
            , m_pGstRequestedTeeSrcPad(NULL)
            , m_pGstStaticNodetrSrcPad(NULL)
            , m_pGstRequestedMuxerSinkPad(NULL)
            , m_pAsyncUnlinkMutex(NULL)
            , m_pAsyncUnlinkCond(NULL)
        {
            LOG_FUNC();

            LOG_DEBUG("New GstNodetr '" << GetName() << "' created");
        }

        /**
         * @brief dtor for the GstNodetr base class
         */
        ~GstNodetr()
        {
            LOG_FUNC();
        
            if (!GetGstElement())
            {
                LOG_WARN("GstElement for GstNodetr '" << GetName() << "' has not been instantiated");
            }
            else
            {
                // Set the State to NULL to free up all resource before removing childern
                LOG_DEBUG("Setting GstElement for GstNodetr '" << GetName() << "' to GST_STATE_NULL");
                gst_element_set_state(GetGstElement(), GST_STATE_NULL);

                // Remove all child references 
                RemoveAllChildren();
                
                if (!m_pParentGstObj)
                {
                    LOG_DEBUG("Unreferencing GST Object contained by this Bintr '" << GetName() << "'");
                    gst_object_unref(m_pGstObj);
                }
            }
            LOG_DEBUG("Nodetr '" << GetName() << "' deleted");
        }

        /**
         * @brief adds a child Bintr to this parent Bintr
         * @param[in] pChildBintr to add. Once added, calling InUse()
         *  on the Child Bintr will return true
         * @return true if pChild was added successfully, false otherwise
         */
        virtual bool AddChild(DSL_BASE_PTR pChild)
        {
            LOG_FUNC();
            
            DSL_NODETR_PTR pChildNodetr = std::dynamic_pointer_cast<Nodetr>(pChild);

            if (!gst_bin_add(GST_BIN(m_pGstObj), pChildNodetr->GetGstElement()))
            {
                LOG_ERROR("Failed to add " << pChildNodetr->GetName() << " to " << GetName() <<"'");
                throw;
            }
            pChildNodetr->m_pParentGstObj = m_pGstObj;
            return Nodetr::AddChild(pChild);
        }
        
        /**
         * @brief removes a child Bintr from this parent Bintr
         * @param[in] pChildBintr to remove. Once removed, calling InUse()
         *  on the Child Bintr will return false
         */
        virtual bool RemoveChild(DSL_BASE_PTR pChild)
        {
            LOG_FUNC();
            
            if (!IsChild(pChild))
            {
                LOG_ERROR("'" << pChild->GetName() << "' is not a child of '" << GetName() <<"'");
                return false;
            }

            DSL_NODETR_PTR pChildNodetr = std::dynamic_pointer_cast<Nodetr>(pChild);
            
            // Increase the reference count so the child is not destroyed.
            gst_object_ref(pChildNodetr->GetGstElement());
            
            if (!gst_bin_remove(GST_BIN(m_pGstObj), pChildNodetr->GetGstElement()))
            {
                LOG_ERROR("Failed to remove " << pChildNodetr->GetName() << " from " << GetName() <<"'");
                return false;
            }
            pChildNodetr->m_pParentGstObj = NULL;
            return Nodetr::RemoveChild(pChildNodetr);
        }

        /**
         * @brief removed a child Nodetr of this parent Nodetr
         * @param[in] pChild to remove
         */
        void RemoveAllChildren()
        {
            LOG_FUNC();

            for (auto &imap: m_pChildren)
            {
                LOG_DEBUG("Removing Child GstNodetr'" << imap.second->GetName() << "' from Parent GST BIn'" << GetName() <<"'");
                
                DSL_NODETR_PTR pChildNodetr = std::dynamic_pointer_cast<Nodetr>(imap.second);

                // Increase the reference count so the child is not destroyed.
                gst_object_ref(pChildNodetr->GetGstElement());

                if (!gst_bin_remove(GST_BIN(m_pGstObj), pChildNodetr->GetGstElement()))
                {
                    LOG_ERROR("Failed to remove GstNodetr " << pChildNodetr->GetName() << " from " << GetName() <<"'");
                }
                pChildNodetr->m_pParentGstObj = NULL;
            }
            Nodetr::RemoveAllChildren();
        }
        
        /**
         * @brief Creates a new Ghost Sink pad for this Gst Element
         * @param[in] padname which pad to add to, either "sink" or "src"
         * @throws a general exception on failure
         */
        void AddGhostPadToParent(const char* padname)
        {
            LOG_FUNC();

            // create a new ghost pad with the static Sink pad retrieved from this Elementr's 
            // pGstObj and adds it to the the Elementr's Parent Bintr's pGstObj.
            if (!gst_element_add_pad(GST_ELEMENT(GetParentGstObject()), 
                gst_ghost_pad_new(padname, gst_element_get_static_pad(GetGstElement(), padname))))
            {
                LOG_ERROR("Failed to add Pad '" << padname << "' for element'" << GetName() << "'");
                throw;
            }
        }

        /**
         * @brief links this Elementr as Source to a given Sink Elementr
         * @param[in] pSink to link to
         */
        bool LinkToSink(DSL_NODETR_PTR pSink)
        { 
            LOG_FUNC();
            
            // Call the base class to setup the relationship first
            // Then call GST to Link Source Element to Sink Element 
            if (!Nodetr::LinkToSink(pSink) or !gst_element_link(GetGstElement(), m_pSink->GetGstElement()))
            {
                LOG_ERROR("Failed to link " << GetName() << " to " << pSink->GetName());
                return false;
            }
            return true;
        }

        /**
         * @brief unlinks this Nodetr from a previously linked-to Sink Notetr
         */
        bool UnlinkFromSink()
        { 
            LOG_FUNC();

            // Need to check here first, as we're calling the base class last when unlinking
            if (!IsLinkedToSink())
            {
                LOG_ERROR("GstNodetr '" << GetName() << "' is not in a linked state");
                return false;
            }
            if (!GetGstElement() or !m_pSink->GetGstElement())
            {
                LOG_ERROR("Invalid GstElements for  '" << GetName());
                return false;
            }
            gst_element_unlink(GetGstElement(), m_pSink->GetGstElement());
            return Nodetr::UnlinkFromSink();
        }

        /**
         * @brief links this Elementr as Sink to a given Source Nodetr
         * @param[in] pSinkBintr to link to
         */
        bool LinkToSource(DSL_NODETR_PTR pSrc)
        { 
            LOG_FUNC();
            
            DSL_NODETR_PTR pSrcNodetr = std::dynamic_pointer_cast<Nodetr>(pSrc);

            // Call the base class to setup the relationship first
            // Then call GST to Link Source Element to Sink Element 
            if (!Nodetr::LinkToSource(pSrcNodetr) or 
                !gst_element_link(pSrcNodetr->GetGstElement(), GetGstElement()))
            {
                LOG_ERROR("Failed to link Source '" << pSrcNodetr->GetName() 
                    << " to Sink" << GetName());
                return false;
            }
            return true;
        }

        /**
         * @brief returns the currently linked to Sink Nodetr
         * @return shared pointer to Sink Nodetr, nullptr if Unlinked to sink
         */
        DSL_GSTNODETR_PTR GetGstSource()
        {
            LOG_FUNC();
            
            return std::dynamic_pointer_cast<GstNodetr>(m_pSrc);
        }

        /**
         * @brief unlinks this Elementr from a previously linked-to Source Element
         */
        bool UnlinkFromSource()
        { 
            LOG_FUNC();

            // Need to check here first, as we're calling the base class last when unlinking
            if (!IsLinkedToSource())
            {
                LOG_ERROR("GstNodetr '" << GetName() << "' is not in a linked state");
                return false;
            }
            if (!m_pSrc->GetGstElement() or !GetGstElement())
            {
                LOG_ERROR("Invalid GstElements for  '" << m_pSrc->GetName() 
                    << "' and '" << GetName() << "'");
                return false;
            }
            gst_element_unlink(m_pSrc->GetGstElement(), GetGstElement());

            return Nodetr::UnlinkFromSource();
        }

        /**
         * @brief links this Noder to the Sink Pad of Muxer
         * @param[in] pMuxer nodetr to link to
         * @param[in] padName name to give the requested Sink Pad
         * @return true if able to successfully link with Muxer Sink Pad
         */
        bool LinkToSinkMuxer(DSL_NODETR_PTR pMuxer, const char* padName)
        {
            LOG_FUNC();
            
            m_pGstStaticNodetrSrcPad = gst_element_get_static_pad(GetGstElement(), "src");
            if (!m_pGstStaticNodetrSrcPad)
            {
                LOG_ERROR("Failed to get Static Src Pad for Bintr '" << GetName() << "'");
                return false;
            }

            m_pGstRequestedMuxerSinkPad = gst_element_get_request_pad(
                pMuxer->GetGstElement(), padName);
            if (!m_pGstRequestedMuxerSinkPad)
            {
                LOG_ERROR("Failed to get requested Tee Sink Pad for Bintr '" 
                    << GetName() <<"'");
                return false;
            }
            
            LOG_INFO("Linking requested Sink Pad'" << m_pGstRequestedMuxerSinkPad 
                << "' for Bintr '" << GetName() << "'");
                
            if (gst_pad_link(m_pGstStaticNodetrSrcPad, 
                m_pGstRequestedMuxerSinkPad) != GST_PAD_LINK_OK)
            {
                LOG_ERROR("Bintr '" << GetName() << "' failed to link to Sink Muxer Tee");
                return false;
            }
            return Nodetr::LinkToSink(pMuxer);
        }
        
        /**
         * @brief unlinks this Nodetr from a previously linked Muxer Sink Pad
         * @return true if able to successfully unlink from Muxer Sink Pad
         */
        bool UnlinkFromSinkMuxer()
        {
            LOG_FUNC();
            
            if (!IsLinkedToSink())
            {
                return false;
            }
            LOG_INFO("Unlinking and releasing requested Sink Pad '" 
                << m_pGstRequestedMuxerSinkPad << "' for Bintr '" << GetName() << "'");

            GstStateChangeReturn changeResult = gst_element_set_state(GetGstElement(), 
                GST_STATE_NULL);
            switch (changeResult)
            {
            case GST_STATE_CHANGE_FAILURE:
                LOG_ERROR("Bintr '" << GetName() << "' failed to set state to NULL");
                return false;

            case GST_STATE_CHANGE_ASYNC:
                LOG_INFO("Bintr '" << GetName() << "' changing state to NULL async");
                // block on get state until change completes. 
                if (gst_element_get_state(GetGstElement(), NULL, NULL, GST_CLOCK_TIME_NONE) 
                    == GST_STATE_CHANGE_FAILURE)
                {
                    LOG_ERROR("Bintr '" << GetName() << "' failed to set state to NULL");
                    return false;
                }
                // drop through on success - DO NOT BREAK

            case GST_STATE_CHANGE_SUCCESS:
                LOG_INFO("Bintr '" << GetName() 
                    << "' changed state to NULL successfully");
                    
                gst_pad_send_event(m_pGstRequestedMuxerSinkPad, gst_event_new_flush_stop(FALSE));
                if (!gst_pad_unlink(m_pGstStaticNodetrSrcPad, m_pGstRequestedMuxerSinkPad))
                {
                    LOG_ERROR("Bintr '" << GetName() << "' failed to unlink from Sink Muxer");
                    Nodetr::UnlinkFromSink();
                    return false;
                }
                gst_element_release_request_pad(GetSink()->GetGstElement(), 
                    m_pGstRequestedMuxerSinkPad);
                gst_object_unref(m_pGstStaticNodetrSrcPad);
                gst_object_unref(m_pGstRequestedMuxerSinkPad);
                return Nodetr::UnlinkFromSink();
            default:
                break;
            }
            LOG_ERROR("Unknown state change for Bintr '" << GetName() << "'");
            return false;

        }
        
        /**
         * @brief links this Nodetr as a Sink to the Source Pad of a Splitter Tee
         * @param[in] pTee to link to
         * @param[in] padName name to give the requested Src Pad
         * @return true if able to successfully link with Tee Src Pad
         */
        virtual bool LinkToSourceTee(DSL_NODETR_PTR pTee, const char* padName)
        {
            LOG_FUNC();
            
            m_pGstStaticNodetrSinkPad = gst_element_get_static_pad(GetGstElement(), "sink");
            if (!m_pGstStaticNodetrSinkPad)
            {
                LOG_ERROR("Failed to get Static Sink Pad for Bintr '" << GetName() << "'");
                return false;
            }

            m_pGstRequestedTeeSrcPad = gst_element_get_request_pad(pTee->GetGstElement(), padName);
                
            if (!m_pGstRequestedTeeSrcPad)
            {
                LOG_ERROR("Failed to get Tee source Pad for Tee '" << GetName() <<"'");
                return false;
            }
            LOG_INFO("Linking requested Src Pad'" << m_pGstRequestedTeeSrcPad 
                << "' for Bintr '" << GetName() << "'");
            if (gst_pad_link(m_pGstRequestedTeeSrcPad, m_pGstStaticNodetrSinkPad) != GST_PAD_LINK_OK)
            {
                LOG_ERROR("Bintr '" << GetName() << "' failed to link to Source Tee");
                return false;
            }
            return Nodetr::LinkToSource(pTee);
        }
        
        /**
         * @brief unlinks this Nodetr from a previously linked Source Tee
         * @return true if able to successfully unlink from Source Tee
         */
        virtual bool UnlinkFromSourceTee()
        {
            LOG_FUNC();
            
            if (!IsLinkedToSource())
            {
                return false;
            }
            GstState state;
            GetState(state, 100);
            if (state == GST_STATE_PLAYING)
            {
                m_pAsyncUnlinkMutex = g_new(GMutex, 1);
                g_mutex_init(m_pAsyncUnlinkMutex);
                g_mutex_lock(m_pAsyncUnlinkMutex);

                m_pAsyncUnlinkCond = g_new(GCond, 1);
                g_cond_init(m_pAsyncUnlinkCond);

                gst_pad_add_probe(m_pGstRequestedTeeSrcPad, GST_PAD_PROBE_TYPE_BLOCK_DOWNSTREAM,
                    (GstPadProbeCallback)complete_unlink_from_source_tee_cb, this, NULL);

                g_cond_wait(m_pAsyncUnlinkCond, m_pAsyncUnlinkMutex);
                g_cond_clear(m_pAsyncUnlinkCond);
                g_free(m_pAsyncUnlinkCond);
                m_pAsyncUnlinkCond = NULL;
                g_mutex_unlock(m_pAsyncUnlinkMutex);
                g_mutex_clear(m_pAsyncUnlinkMutex);
                g_free(m_pAsyncUnlinkMutex);
                m_pAsyncUnlinkMutex == NULL;
            }
            else
            {
                CompleteUnlinkFromSourceTee();
            }
            return Nodetr::UnlinkFromSource();

        }

        void CompleteUnlinkFromSourceTee()
        {
            if (m_pAsyncUnlinkMutex and m_pAsyncUnlinkCond)
            {
                g_mutex_lock(m_pAsyncUnlinkMutex);
            }

            LOG_INFO("Unlinking and releasing requested Src Pad '" 
                << m_pGstRequestedTeeSrcPad << "' for Bintr '" << GetName() << "'");

            if (!gst_pad_unlink(m_pGstRequestedTeeSrcPad, m_pGstStaticNodetrSinkPad))
            {
                LOG_ERROR("Bintr '" << GetName() << "' failed to unlink from Source Tee");
                return;
            }
            GstState state;
            GetState(state, 100);
            if (state == GST_STATE_PLAYING)
            {
                SetState(GST_STATE_NULL, 100);
            }
            gst_element_release_request_pad(GetSource()->GetGstElement(), m_pGstRequestedTeeSrcPad);
            gst_object_unref(m_pGstStaticNodetrSinkPad);
            gst_object_unref(m_pGstRequestedTeeSrcPad);
            m_pGstStaticNodetrSinkPad = m_pGstRequestedTeeSrcPad = NULL;

            if (m_pAsyncUnlinkMutex and m_pAsyncUnlinkCond)
            {
                g_cond_signal(m_pAsyncUnlinkCond);
                g_mutex_unlock(m_pAsyncUnlinkMutex);
            }
        }
        
        /**
         * @brief Returns the current State of this Bintr's Parent
         * @return the current state of the Parenet, GST_STATE_NULL if the
         * GstNodetr is currently an orphen. 
         */
        uint GetParentState()
        {
            LOG_FUNC();
            
            if (!m_pParentGstObj)
            {
                return GST_STATE_NULL;
            }
            GstState currentState;
            
            gst_element_get_state(GetParentGstElement(), &currentState, NULL, 1);
            
            LOG_DEBUG("Returning a state of '" << gst_element_state_get_name(currentState) 
                << "' for Nodetr '" << GetName());
            
            return currentState;
        }

        bool SendEos()
        {
            LOG_FUNC();
            
            return gst_element_send_event(GetGstElement(), gst_event_new_eos());
        }

        uint GetState(GstState& state, GstClockTime timeout)
        {
            LOG_FUNC();

            uint retval = gst_element_get_state(GetGstElement(), &state, NULL, timeout);
            LOG_DEBUG("Get state returned '" << gst_element_state_get_name(state) 
                << "' for Bintr '" << GetName() << "'");
            
            return retval;
        }
        
        /**
         * @brief Attempts to set the state of this Bintr's GST Element
         * @return true if successful transition, false on failure
         */
        bool SetState(GstState state, GstClockTime timeout)
        {
            LOG_FUNC();
            LOG_INFO("Changing state to '" << gst_element_state_get_name(state) 
                << "' for Bintr '" << GetName() << "'");

            GstStateChangeReturn returnVal = gst_element_set_state(GetGstElement(), state);
            switch (returnVal) 
            {
                case GST_STATE_CHANGE_SUCCESS:
                    LOG_INFO("State change completed synchronously for Bintr'" << GetName() << "'");
                    return true;
                case GST_STATE_CHANGE_FAILURE:
                    LOG_ERROR("FAILURE occured when trying to change state to '" << 
                        gst_element_state_get_name(state) << "' for Bintr '" << GetName() << "'");
                    return false;
                case GST_STATE_CHANGE_NO_PREROLL:
                    LOG_INFO("Set state for Bintr '" << GetName() 
                        << "' returned GST_STATE_CHANGE_NO_PREROLL");
                    return true;
                case GST_STATE_CHANGE_ASYNC:
                    LOG_INFO("State change will complete asynchronously for Bintr '" << GetName() << "'");
                    break;
                default:
                    break;
            }
            
            // Wait until state change or failure, no timeout.
            if (gst_element_get_state(GetGstElement(), NULL, NULL, timeout) == GST_STATE_CHANGE_FAILURE)
            {
                LOG_ERROR("FAILURE occured waiting for state to change to '" 
                    << gst_element_state_get_name(state) << "' for Bintr '" << GetName() << "'");
                return false;
            }
            LOG_INFO("State change completed asynchronously for Bintr'" << GetName() << "'");
            return true;
        }

        uint SyncStateWithParent(GstState& parentState, GstClockTime timeout)
        {
            LOG_FUNC();
            
            uint returnVal = gst_element_sync_state_with_parent(GetGstElement());

            switch (returnVal) 
            {
                case GST_STATE_CHANGE_SUCCESS:
                    LOG_INFO("State change completed synchronously for Bintr'" << GetName() << "'");
                    return returnVal;
                case GST_STATE_CHANGE_FAILURE:
                    LOG_ERROR("FAILURE occured when trying to sync state with Parent for Bintr '" 
                        << GetName() << "'");
                    return returnVal;
                case GST_STATE_CHANGE_NO_PREROLL:
                    LOG_INFO("Set state for Bintr '" << GetName() << "' return GST_STATE_CHANGE_NO_PREROLL");
                    return returnVal;
                case GST_STATE_CHANGE_ASYNC:
                    LOG_INFO("State change will complete asynchronously for Bintr '" << GetName() << "'");
                    break;
                default:
                    break;
            }
            uint retval = gst_element_get_state(GST_ELEMENT_PARENT(GetGstElement()), 
                &parentState, NULL, timeout);
            LOG_INFO("Get state returned '" << gst_element_state_get_name(parentState) 
                << "' for Parent of Bintr '" << GetName() << "'");
            return retval;
        }
        

    protected:

        /**
         * @brief Static sink pad for the Nodetr used to link to a Splitter Tee.
         */
        GstPad* m_pGstStaticNodetrSinkPad;

        /**
         * @brief requested Src Pad when linking to a Splitter Tee used.
         */
        GstPad* m_pGstRequestedTeeSrcPad;

        /**
         * @brief Static Src Pad for this Nodetr used to link to a Muxer Tee.
         */
        GstPad* m_pGstStaticNodetrSrcPad;

        /**
         * @brief requested Sink Pad for the Muxer Tee linked to the VideoSource.
         */
        GstPad* m_pGstRequestedMuxerSinkPad;
        /**
         * @brief Mutex to syncronize the dynamic unlink process
         * when the Nodetr in a playing state
         */
        GMutex* m_pAsyncUnlinkMutex;
        
        /**
         * @brief Condition to syncronize the dynamic unlink process
         * when the Nodetr in a playing state
         */
        GCond* m_pAsyncUnlinkCond;
    };

    static GstPadProbeReturn complete_unlink_from_source_tee_cb(GstPad* pad, 
        GstPadProbeInfo *info, gpointer pNoder)
    {
        static_cast<GstNodetr*>(pNoder)->CompleteUnlinkFromSourceTee();

        return GST_PAD_PROBE_REMOVE;
    }

} // DSL namespace    

#endif // _DSL_NODETR_H