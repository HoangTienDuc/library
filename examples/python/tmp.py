import sys
from dsl import *

# RTSP Source URI for HIKVISION Camera    
hikvision_rtsp_uri = 'rtsp://admin:123456Aa@kybernetwork123.cameraddns.net:1554/Streaming/Channels/601'    

primary_infer_config_file_dgpu = '/opt/nvidia/deepstream/deepstream/samples/configs/deepstream-app/config_infer_primary.txt'
primary_model_engine_file_dgpu = '/opt/nvidia/deepstream/deepstream/samples/trtis_model_repo/Primary_Detector/1/resnet10.caffemodel_b30_gpu0_int8.engine'

# Filespec for the IOU Tracker config file
iou_tracker_config_file = '/opt/nvidia/deepstream/deepstream/samples/configs/deepstream-app/config_tracker_IOU.yml'

TILER_WIDTH = DSL_STREAMMUX_DEFAULT_WIDTH
TILER_HEIGHT = DSL_STREAMMUX_DEFAULT_HEIGHT
WINDOW_WIDTH = TILER_WIDTH
WINDOW_HEIGHT = TILER_HEIGHT

while True:
    retval = dsl_source_rtsp_new('rtsp-source',     
        uri=hikvision_rtsp_uri,     
        protocol=DSL_RTP_ALL,     
        skip_frames=0,     
        drop_frame_interval=0,     
        latency=20000,
        timeout=20)    
    if retval != DSL_RETURN_SUCCESS:    
        break

    # Create the Primary GIE, Tracker, Multi-Source Tiler, On-Screen Display and X11/EGL Window Sink
    retval = dsl_infer_gie_primary_new('pgie', 
                primary_infer_config_file_dgpu, primary_model_engine_file_dgpu, 4)
    if (retval != DSL_RETURN_SUCCESS):
        break

    retval = dsl_tracker_new('iou-tracker', iou_tracker_config_file, 480, 272)
    if (retval != DSL_RETURN_SUCCESS):
        break

    retval = dsl_tiler_new('tiler', width=1280, height=720)
    if (retval != DSL_RETURN_SUCCESS):
        break

    retval = dsl_osd_new('osd', text_enabled=True, clock_enabled=True,
        bbox_enabled=True, mask_enabled=False)
    if (retval != DSL_RETURN_SUCCESS):
        break
    host_uri = 'uubpro.local'
    retval = dsl_sink_rtsp_new('rtsp-sink', 
        host=host_uri, udp_port=5400, rtsp_port=8554, codec=DSL_CODEC_H265, bitrate=200000, interval=0)
    if (retval != DSL_RETURN_SUCCESS):
        break

    # New Window Sink, with matching dimensions as the Tiler
    retval = dsl_sink_window_new('window-sink', 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)
    if retval != DSL_RETURN_SUCCESS:
        break
    
    # New Splitter Tee- 
    retval = dsl_tee_splitter_new('splitter')
    if retval != DSL_RETURN_SUCCESS:
        break

    # Add Branch1 and the demuxer (as branch2) to the splitter
    retval = dsl_tee_branch_add_many('splitter', ['rtsp-sink', 'window-sink', None])
    if retval != DSL_RETURN_SUCCESS:
        break


    # Create a Pipeline and add the new components.
    retval = dsl_pipeline_new_component_add_many('pipeline', 
        components=['rtsp-source', 'pgie', 'iou-tracker', 'tiler', 'osd', 'splitter', None]) 
    if (retval != DSL_RETURN_SUCCESS):
        break
   
    # Object Detection Event (ODE) Pad Probe Handler (PPH) to manage our ODE Triggers with their ODE Actions
    retval = dsl_pph_ode_new('ode-handler')
    if (retval != DSL_RETURN_SUCCESS):
        break
 
    # Add the ODE Handler to the Sink (input) pad of the Tiler - before the batched frames are combined/tiled
    retval = dsl_tiler_pph_add('tiler', 'ode-handler', DSL_PAD_SINK)
    if (retval != DSL_RETURN_SUCCESS):
        break
    
    # Pipeline has been successfully created, ok to play
    retval = dsl_pipeline_play('pipeline')
    if (retval != DSL_RETURN_SUCCESS):
        break

    # join the main loop until stopped. 
    dsl_main_loop_run()
    break

# Print out the final result
print(dsl_return_value_to_string(retval))

# free up all resources
dsl_delete-all()