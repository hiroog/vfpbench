# 2018 Hiroyuki Ogasawara
# vim:ts=4 sw=4 et:

#  $ flmake
#  $ flmake run
#  $ more output_log.txt
#  $ flamke list
#
# debug build
#  $ flamke debug
#
# Show CPU information
#  $ ./vfpbench -i

import re

#------------------------------------------------------------------------------

src_list= [
        'src/linux_main.cpp',
        'src/TestBase.cpp',
        'src/SSE32SP_Test.cpp',
        'src/SSE32DP_Test.cpp',
        'src/SSE64SP_Test.cpp',
        'src/SSE64DP_Test.cpp',
        'src/SSE64BF16_Test.cpp',
        'src/SSE64INT8_Test.cpp',
        'src/VFP32SP_Test.cpp',
        'src/VFP32DP_Test.cpp',
        'src/VFP64SP_Test.cpp',
        'src/VFP64DP_Test.cpp',
        'src/VFP64HP_Test.cpp',
        'src/VFP64BF16_Test.cpp',
        'src/VFP64INT8_Test.cpp',
        'src/MIPSSP_Test.cpp',
        'src/MIPSDP_Test.cpp',
        'src/ResultData.cpp',
        'src/BenchApplication.cpp',
        'src/BenchmarkTest.cpp',
    ]

TargetName= 'vfpbench'


#------------------------------------------------------------------------------

tool.addCleanTask( genv, 'clean' )

#------------------------------------------------------------------------------

def get_macos_arm64_arch():
    import subprocess
    def has_feat( key ):
        try:
            result= subprocess.run( ['sysctl', '-n', 'hw.optional.arm.' + key], capture_output=True, text=True )
            return result.stdout.strip() == '1'
        except:
            return False
    ext= ''
    if has_feat( 'FEAT_FP16' ):
        ext+= '+fp16'
    if has_feat( 'FEAT_BF16' ):
        ext+= '+bf16'
    if has_feat( 'FEAT_I8MM' ):
        ext+= '+i8mm'
    if has_feat( 'FEAT_DotProd' ):
        ext+= '+dotprod'
    return  'armv8.2-a+simd' + ext

def get_linux_arm64_arch():
    base_arch= 'armv8-a+simd'
    with open( '/proc/cpuinfo', 'r' ) as fi:
        for line in fi:
            if line.startswith( 'Features' ) or line.startswith( 'flags' ):
                feature_set= set(line.split())
                if 'sve2' in feature_set:
                    base_arch= 'armv9-a'
                if ('fphp' in feature_set) and ('asimdhp' in feature_set):
                    base_arch+= '+fp16'
                if 'asimddp' in feature_set:
                    base_arch+= '+dotprod'
                if 'i8mm' in feature_set:
                    base_arch+= '+i8mm'
                if 'bf16' in feature_set:
                    base_arch+= '+bf16'
                break
    return  base_arch

#------------------------------------------------------------------------------

if os.path.exists( 'src/flatlib' ):
    FLATLIB_ROOT= 'src'
else:
    FLATLIB_ROOT= tool.findPath( '../../flatlib5', 'FLATLIB5' )

flatlib_base_list= [
        'flatlib/core/memory/MemoryBuffer.cpp',
        'flatlib/core/memory/MemoryAllocator.cpp',
        'flatlib/core/memory/MemoryAddress.cpp',
        'flatlib/core/system/ConsoleLog.cpp',
        'flatlib/core/system/Assert.cpp',
        'flatlib/core/system/CoreContext.cpp',
        'flatlib/core/system/SystemInfo.cpp',
        'flatlib/core/time/DateTime.cpp',
        'flatlib/core/time/SystemClock.cpp',
        'flatlib/core/thread/Sleep.cpp',
        'flatlib/core/thread/Mutex.cpp',
        'flatlib/core/thread/CriticalSection.cpp',
        'flatlib/core/thread/ThreadInstance.cpp',
        'flatlib/core/thread/Processor.cpp',
        'flatlib/core/math/Float.cpp',
        'flatlib/core/math/Matrix4.cpp',
        'flatlib/core/ut/BinaryPool.cpp',
        'flatlib/core/platform/WinString.cpp',
        'flatlib/core/text/TextPool.cpp',
    ]

#------------------------------------------------------------------------------

env= tool.createTargetEnvironment()
env.addIncludePaths( [ 'src', FLATLIB_ROOT ] )
env.addCCFlags( ['-DFL_USE_FILESYSTEM=0', '-DFL_USE_DATABASE=0', '-DFL_USE_VECT4=0'] )

if env.getHostPlatform() == 'macOS':
    flatlib_base_list.append( 'flatlib/core/system/SystemInfo_darwin.mm' )
flatlib_src_list= [ os.path.join( FLATLIB_ROOT, src ) for src in flatlib_base_list ]
src_list.extend( flatlib_src_list )

env.refresh()

def addCustomBuild( env, TargetName, src_list, config ):
    global FLATLIB_ROOT
    is_termux= False
    if env.getHostPlatform() == 'Linux':
        is_termux= env.isTermux()
    tool= env.tool
    #arch_list= env.getSupportArchList()
    arch_list= [ env.getHostArch() ]
    task_list= []
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
            if local_env.getTargetPlatform() == 'macOS':
                global get_macos_arm64_arch
                local_env.addCCFlags( ['-march=' + get_macos_arm64_arch()] )
            else:
                global get_linux_arm64_arch
                local_env.addCCFlags( ['-march=' + get_linux_arm64_arch()] )
        elif arch == 'x64':
            local_env.addCCFlags( ['-march=native'] )
        if config == 'Release':
            exe_name= TargetName
        else:
            local_env.addCCFlags( ['-DDEBUG=1'] )
            exe_name= env.getExeName( TargetName + '_' + env.getTargetArch() + '_' + env.getConfig() )
        local_env.refresh()
        task= tool.addExeTask( local_env, None, src_list, target=exe_name )
        task_list.append( task )
    return  task_list

task_list= addCustomBuild( env, TargetName, src_list, 'Release' )
tool.addGroupTask( env, 'build', task_list )

task_list= addCustomBuild( env, TargetName, src_list, 'Debug' )
tool.addGroupTask( env, 'debug', task_list )


#------------------------------------------------------------------------------

def BackupSaveFile():
    if os.path.exists( '.save.log' ):
        if not os.path.exists( 'save' ):
            os.mkdir( 'save' )
        global  re
        date_pat= re.compile( r'^T\s+"([0-9]+)\s+([0-9]+)"$' )
        nname= 'save0.log'
        with open( '.save.log', 'r' ) as fi:
            for line in fi:
                pat= date_pat.search( line )
                if pat is not None:
                    fdate= pat.group( 1 )
                    ftime= pat.group( 2 )
                    nname= 'save_%s_%s.log' % (fdate, ftime)
                    break
        os.rename( '.save.log', os.path.join( 'save', nname ) )

def BenchRun_Linux( task ):
    global BackupSaveFile
    BackupSaveFile()
    if os.path.exists( 'vfpbench' ):
        import  subprocess
        proc= subprocess.Popen( ['./vfpbench'] )
        proc.wait()


#------------------------------------------------------------------------------

def decode_benchmark_list( bench_text ):
    global re
    bench_id_list= []
    bench_pat= re.compile( '^\s*(\d+):\s+G=(\d+)\s+T=(\d+)\s+' )
    for line in bench_text.split( '\n' ):
        pat= bench_pat.search( line )
        if pat:
            bench_id= int(pat.group( 1 ))
            core_group= int(pat.group( 2 ))
            multi_thread= int(pat.group( 3 ))
            bench_id_list.append( { 'id': bench_id, 'group': core_group, 'thread': multi_thread } )
    return  bench_id_list

def get_core_groups():
    import  subprocess
    group_name_list= []
    for gi in range(8):
        result= subprocess.run( ['sysctl', '-n', 'hw.perflevel%d.name' % gi], capture_output=True, text=True )
        name= result.stdout.strip()
        if name != '':
            group_name_list.append( (gi,name) )
    return  group_name_list

def BenchRun_Mac( task ):
    BackupSaveFile()
    if not os.path.exists( 'vfpbench' ):
        return
    import  subprocess
    result= subprocess.run( ['./vfpbench', '-l'], capture_output=True, text=True )
    global decode_benchmark_list
    benchmark_list= decode_benchmark_list( result.stdout )
    global get_core_groups
    group_name_list= get_core_groups()
    for gi,group_name in group_name_list:
        for bi,bench_map in enumerate(benchmark_list):
            if bench_map['group'] == gi:
                if group_name == 'Performance':
                    subprocess.run( ['./vfpbench', '-b%d' % bi] )
                elif group_name == 'Efficiency':
                    subprocess.run( ['taskpolicy', '-c', 'background', './vfpbench', '-b%d' % bi] )


#------------------------------------------------------------------------------

def BenchRun( task ):
    if task.env.getHostPlatform() == 'macOS':
        global BenchRun_Mac
        BenchRun_Mac( task )
    else:
        global BenchRun_Linux
        BenchRun_Linux( task )

task= tool.addScriptTask( env, 'run', BenchRun )


#------------------------------------------------------------------------------

def GetName():
    src_file= 'output_log.txt'
    name= ''
    with open( src_file, 'r' ) as fi:
        name_pat= re.compile( '^Name: (.*)$' )
        for line in fi:
            pat= name_pat.search( line )
            if pat is not None:
                name= pat.group(1).strip()
                break
    if os.path.exists( '.processor_name' ):
        with open( '.processor_name', 'r' ) as fi:
            name= fi.readline().strip()
            if name.startswith( 'NAME:' ):
                name= name[5:].strip()
    name= name.replace( '/', '_' )
    return  name

def PushLog( task ):
    global re
    import datetime
    src_file= 'output_log.txt'
    fdate= datetime.datetime.fromtimestamp( os.stat( src_file ).st_mtime ).strftime( '%Y%m%d_%H%M%S' )
    global GetName
    name= GetName()
    if name == '':
        return
    if not os.path.exists( 'log' ):
        os.mkdir( 'log' )
    file_name= os.path.join( 'log', '%s.txt' % name )
    import shutil
    shutil.copy( src_file, file_name )
    if os.path.exists( '.processor_name' ):
        with open( '.processor_name', 'r' ) as fi:
            processor= fi.read()
        with open( file_name, 'a' ) as fa:
            fa.write( '\n\n' )
            fa.write( processor )
            fa.write( '\n\n' )
    print( file_name )

task= tool.addScriptTask( env, 'push', PushLog )


#------------------------------------------------------------------------------

def ListLog( task ):
    log_list= os.listdir( 'log' )
    global re
    import urllib.parse
    plist= [
            re.compile( r'^SingleThread\s+HP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+SP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+DP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+BF16\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^SingleThread\s+INT8\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+HP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+SP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+DP\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+BF16\s+max:\s+([0-9.-]+)' ),
            re.compile( r'^MultiThread\s+INT8\s+max:\s+([0-9.-]+)' ),
        ]
    sort_key_map= {
        's-half': 0, 's-float': 1, 's-double': 2, 's-bf16': 3, 's-int8': 4,
        'm-half': 5, 'm-float': 6, 'm-double': 7, 'm-bf16': 8, 'm-int8': 9,
    }
    sort_index= sort_key_map['m-float']
    if hasattr( task, 'sort_key' ):
        sort_index= sort_key_map[task.sort_key]
    thread_pat= re.compile( r'^CPU\s+Thread:\s+([0-9]+)' )
    core_pat=   re.compile( r'^CPU\s+Core\s+:\s+([0-9]+)' )
    clock_pat=  re.compile( r'^\s+Group\s+[0-9]+\s*:.*Clock=\s*([0-9.-]+)\s*GHz' )
    ext_ignore= { '.swp' }
    src_file= 'output_log.txt'
    if os.path.exists( src_file ):
        log_list.append( '=====> '+ src_file )
    device_list= []
    for log in log_list:
        score= [0.0]*10
        min_clock= 1e30
        max_clock= 0
        _,ext= os.path.splitext(log)
        if ext in ext_ignore:
            continue
        if log[0] == '=':
            log_path= log.strip( ' =>' )
        else:
            log_path= os.path.join( 'log', log )
        with open( log_path, 'r' ) as fi:
            for line in fi:
                for index,p in enumerate(plist):
                    pat= p.search( line )
                    if pat is not None:
                        flops= pat.group(1)
                        if flops == '-':
                            score[index]= 0.0
                        else:
                            score[index]= float(flops)
                        break
                pat= thread_pat.search( line )
                if pat is not None:
                    cpu_thread= int(pat.group(1))
                pat= core_pat.search( line )
                if pat is not None:
                    cpu_core= int(pat.group(1))
                pat= clock_pat.search( line )
                if pat is not None:
                    cpu_clock= float(pat.group(1))
                    if cpu_clock < min_clock:
                        min_clock= cpu_clock
                    if cpu_clock > max_clock:
                        max_clock= cpu_clock
        name,_= os.path.splitext( log )
        device_list.append( (name,score,log,cpu_thread,cpu_core,min_clock,max_clock) )
    device_list_sp= sorted( device_list, key=lambda a: a[1][sort_index], reverse=True )
    if task.table:
        print( '^ Device  ^ Clock ^ Thread ^  Single Thread                      ^^^  Multi Thread                     ^^^' )
        print( '^ :::     ^ :::   ^ :::    ^  Half-p    ^   Single-p  ^  Double-p  ^  Half-p   ^  Single-p  ^  Double-p  ^' )
    else:
        print( 'Device                                              Clock  C/T     S-HP    S-SP    S-DP  S-BF16  S-INT8     M-HP    M-SP    M-DP  M-BF16  M-INT8' )
    for name,sc,log,core,thread,mic,mac in device_list_sp:
        if task.table:
            url= 'https://github.com/hiroog/vfpbench/blob/master/log/' + urllib.parse.quote( log )
            line= '| [[%s|%-70s]]  |  %5.3f GHz |  %d/%d |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |  %8.3f |' % (url,name[:70], mac, thread, core, sc[0], sc[1], sc[2],  sc[5], sc[6], sc[7], sc[3], sc[4], sc[8], sc[9])
            print( line.replace( '0.000', '--' ) )
        else:
            print( '%-50s  %5.3f %2d/%2d %7.2f %7.2f %7.2f %7.2f %7.1f  %7.2f %7.2f %7.2f %7.2f %7.1f' % (name[:50], mac, thread, core, sc[0], sc[1], sc[2], sc[3], sc[4], sc[5], sc[6], sc[7], sc[8], sc[9]) )


def addlist( tool, env, name, key, log, table= False ):
    task= tool.addScriptTask( env, name, log )
    task.table= table
    task.sort_key= key

addlist( tool, env, 'list',         'm-float',  ListLog )
addlist( tool, env, 'list-half',    'm-half',   ListLog )
addlist( tool, env, 'list-float',   'm-float',  ListLog )
addlist( tool, env, 'list-double',  'm-double', ListLog )
addlist( tool, env, 'list-bf16',    'm-bf16',   ListLog )
addlist( tool, env, 'list-int8',    'm-int8',   ListLog )

addlist( tool, env, 'list-s-half',  's-half',   ListLog )
addlist( tool, env, 'list-s-float', 's-float',  ListLog )
addlist( tool, env, 'list-s-double','s-double', ListLog )
addlist( tool, env, 'list-s-bf16',  's-bf16',   ListLog )
addlist( tool, env, 'list-s-int8',  's-int8',   ListLog )

addlist( tool, env, 'table',        's-float',  ListLog, True )

#------------------------------------------------------------------------------
class Converter:
    def __init__( self ):
        global re
        self.date_pat= re.compile( r'(.+)_([0-9]+)_([0-9]+)\.txt' )

    def convet_filename_to_header( self, root, file_name ):
        pat= self.date_pat.search( file_name );
        if pat is None:
            print( 'ERROR', file_name )
            return
        fname= pat.group( 1 )
        fdate= pat.group( 2 )
        ftime= pat.group( 3 )
        full_path= os.path.join( root, file_name )
        new_name= os.path.join( 'tlog', fname + '.txt' )
        with open( full_path, 'r' ) as fi:
            with open( new_name, 'w' ) as fo:
                fo.write( 'Date: %s %s\n' % (fdate,ftime) )
                for line in fi:
                    fo.write( line )

    def walk( self ):
        for root,dirs,files in os.walk( 'log' ):
            for file_name in files:
                self.convet_filename_to_header( root, file_name )


