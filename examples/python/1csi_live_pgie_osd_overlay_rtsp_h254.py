import sys
from dsl import *

# update host URL to 
host_uri = 'uubpro.local'

# RTSP Source URI for HIKVISION Camera    
hikvision_rtsp_uri = 'rtsp://admin:123456Aa@kybernetwork123.cameraddns.net:1554/Streaming/Channels/601'    

primary_infer_config_file_dgpu = '/opt/nvidia/deepstream/deepstream/samples/configs/deepstream-app/config_infer_primary.txt'
primary_model_engine_file_dgpu = '/opt/nvidia/deepstream/deepstream/samples/trtis_model_repo/Primary_Detector/1/resnet10.caffemodel_b30_gpu0_int8.engine'

def main(args):

    # Since we're not using args, we can Let DSL initialize GST on first call
    while True:

        retval = dsl_source_rtsp_new('rtsp-source',     
            uri=hikvision_rtsp_uri,     
            protocol=DSL_RTP_ALL,     
            skip_frames=0,     
            drop_frame_interval=0,     
            latency=100,
            timeout=2)    
        if retval != DSL_RETURN_SUCCESS:    
            return retval    

        # New Primary GIE using the filespecs above, with interval and Id
        retval = dsl_infer_gie_primary_new('primary-gie', 
            primary_infer_config_file_dgpu, primary_model_engine_file_dgpu, 0)
        if retval != DSL_RETURN_SUCCESS:
            break

        # New OSD with text, clock and bbox display all enabled. 
        retval = dsl_osd_new('on-screen-display', 
            text_enabled=True, clock_enabled=True, bbox_enabled=True, mask_enabled=False)
        if retval != DSL_RETURN_SUCCESS:
            break

        retVal = dsl_sink_rtsp_new('rtsp-sink', host_uri, 5400, 8554, DSL_CODEC_H264, 4000000,0)
        if retVal != DSL_RETURN_SUCCESS:
            print(dsl_return_value_to_string(retVal)) 

        # Add all the components to our pipeline
        retval = dsl_pipeline_new_component_add_many('pipeline', 
            ['rtsp-source', 'primary-gie', 'on-screen-display', 'rtsp-sink', None])
        if retval != DSL_RETURN_SUCCESS:
            break

        # Play the pipeline
        retval = dsl_pipeline_play('pipeline')
        if retval != DSL_RETURN_SUCCESS:
            break

        dsl_main_loop_run()
        retval = DSL_RETURN_SUCCESS
        break

    # Print out the final result
    print(dsl_return_value_to_string(retval))

    dsl_delete_all()

if __name__ == '__main__':
    sys.exit(main(sys.argv))
