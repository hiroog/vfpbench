# 2018 Hiroyuki Ogasawara
# vim:ts=4 sw=4 et:

#------------------------------------------------------------------------------

src_list= [
        'src/linux_main.cpp',
        'src/TestBase.cpp',
        'src/MatrixTest_SSESP.cpp',
        'src/MatrixTest_VFP32SP.cpp',
        'src/MatrixTest_VFP64SP.cpp',
        'src/SSE32SP_Test.cpp',
        'src/SSE32DP_Test.cpp',
        'src/SSE64SP_Test.cpp',
        'src/SSE64DP_Test.cpp',
        'src/VFP32SP_Test.cpp',
        'src/VFP32DP_Test.cpp',
        'src/VFP64SP_Test.cpp',
        'src/VFP64DP_Test.cpp',
        'src/VFP64HP_Test.cpp',
        'src/MIPSSP_Test.cpp',
        'src/MIPSDP_Test.cpp',
        'src/ResultData.cpp',
        'src/BenchApplication.cpp',
        'src/BenchmarkTest.cpp',
        'src/minilib/SystemAPI.cpp',
        'src/minilib/Thread.cpp',
        'src/minilib/MemoryAllocator.cpp',
        'src/minilib/Matrix4.cpp',
        'src/minilib/Math.cpp',
        'src/minilib/SystemInfo.cpp',
        'src/minilib/ConsoleIO.cpp',
        'src/minilib/PlatformString.cpp',
    ]


TargetName= 'vfpbench'

#------------------------------------------------------------------------------

def addCleanTask( env, task_name, dir_name= os.getcwd() ):
    def command( task ):
        import  BuildUtility
        BuildUtility.RemoveTree( os.path.join( task.cwd, task.env.OUTPUT_OBJ_DIR ) )
    clean_task= env.tool.addScriptTask( env, task_name, command )
    clean_task.cwd= os.getcwd()
    return  clean_task

addCleanTask( genv, 'clean' )


def makeExeName( env, src_file ):
    if src_file:
        return  env.getExeName( src_file + '_' + env.getTargetArch() + '_' + env.getConfig() )
    return  '.'

#------------------------------------------------------------------------------

env= tool.createTargetEnvironment()
env.EXE_NAME_FUNC= makeExeName
env.addIncludePath( ['src'] )

if env.getHostPlatform() == 'macOS':
    env.addCCFlags( ['-DflPRESET_OSX=1'] )
    if env.getTargetPlatform() == 'macOS' or env.getTargetPlatform() == 'iOS':
        env.addLinkFlags( '-framework Cocoa'.split() )

if env.getHostPlatform() == 'Linux':
    env.addCCFlags( ['-DflPRESET_LINUX=1'] )


env.refresh()

def addCustomBuild( env, TargetName, src_list, is_termux= False ):
    tool= env.tool
    arch_list= env.getSupportArchList()
    config_list= [ 'Debug', 'Release' ]
    task_list= []
    for config in config_list:
        for arch in arch_list:
            local_env= env.clone()
            local_env.setConfig( config )
            local_env.setTargetArch( arch )
            if is_termux:
                if arch == 'arm7':
                    local_env.addCCFlags( ['-mfpu=neon', '-mfloat-abi=softfp'] )
                elif arch == 'x64':
                    local_env.setTargetArch( 'x86' )
            if arch == 'arm64':
                local_env.addCCFlags( ['-march=armv8a+fp16'] )
            if config != 'Release':
                local_env.addCCFlags( ['-DDEBUG=1'] )
            local_env.refresh()
            task= tool.addExeTask( local_env, TargetName, src_list )
            task_list.append( task )
    return  task_list

task_list= addCustomBuild( env, TargetName, src_list )
tool.addNamedTask( env, 'build', task_list )

task_list= addCustomBuild( env, TargetName, src_list, True )
tool.addNamedTask( env, 'termux', task_list )


#------------------------------------------------------------------------------


def BenchRun( task ):
    import  subprocess
    for root,dirs,files in os.walk( '.' ):
        for file_name in files:
            if file_name.startswith( 'vfpbench_' ):
                if file_name.endswith( '_Release' ):
                    proc= subprocess.Popen( ['./'+file_name] )
                    proc.wait()
                    return

task= tool.addScriptTask( env, 'run', BenchRun )


