sampler2D
AlbedoSampler
#define AlbedoSampler() sampler2D( u_instances[gl_InstanceID + gl_BaseInstanceARB].TextureHandles[ 0 ] )