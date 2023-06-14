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


def create_components():
    retval = dsl_source_rtsp_new('rtsp-source',     
        uri=hikvision_rtsp_uri,     
        protocol=DSL_RTP_ALL,     
        skip_frames=0,     
        drop_frame_interval=0,     
        latency=100,
        timeout=2)    
    if retval != DSL_RETURN_SUCCESS:    
        return retval    

    retval = dsl_infer_gie_primary_new('primary-gie', 
        primary_infer_config_file_dgpu, primary_model_engine_file_dgpu, 4)
    if retval != DSL_RETURN_SUCCESS:
        print("[ERROR] Cannot create primary-gie")

    retval = dsl_tracker_new('iou-tracker', iou_tracker_config_file, 480, 272)
    if retval != DSL_RETURN_SUCCESS:
        print("[ERROR] Cannot create iou-tracker")

    retval = dsl_tiler_new('tiler', TILER_WIDTH, TILER_HEIGHT)
    if retval != DSL_RETURN_SUCCESS:
        print("[ERROR] Cannot create tiler")
        
    retval = dsl_osd_new('on-screen-display', 
        text_enabled=True, clock_enabled=True, bbox_enabled=True, mask_enabled=False)
    if retval != DSL_RETURN_SUCCESS:
        print("[ERROR] Cannot create on-screen-display")
        
    retval = dsl_sink_window_new('window-sink', 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT)
    if retval != DSL_RETURN_SUCCESS:
        print("[ERROR] Cannot create window-sink")

    retval = dsl_sink_rtsp_new('rtsp-sink', 
        host='0.0.0.0', udp_port=5400, rtsp_port=8554, codec=DSL_CODEC_H265, bitrate=200000, interval=0)
    if (retval != DSL_RETURN_SUCCESS):
        print("[ERROR] Cannot create rtsp-sink")

    
    # Create a Pipeline and add the new components.
    retval = dsl_pipeline_new_component_add_many('pipeline', 
        ['rtsp-source', 'primary-gie', 'iou-tracker', 'tiler', 
        'on-screen-display', 'window-sink', 'rtsp-sink', None])
    if retval != DSL_RETURN_SUCCESS:
        print("[ERROR] Cannot create primary-gie")

def main():
    create_components()
    retval = dsl_pipeline_play('pipeline')
    if retval != DSL_RETURN_SUCCESS:
        print("[ERROR] Cannot play the pipeline")
    dsl_main_loop_run()
    retval = DSL_RETURN_SUCCESS
    
if __name__ == '__main__':
    sys.exit(main())
