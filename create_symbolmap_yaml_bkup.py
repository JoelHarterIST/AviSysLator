import subprocess
import pandas as pd
import yaml
from enum import IntEnum, auto
import sys

element_type_keys = ["int32_t", "double"]
element_type_keys_math = ["vec3_t", "mat33_t", "quat_t"]
element_type_size_dict = {"int32_t": 4,
                          "double":  8,
                          "vec3_t":  8*3,
                          "mat33_t": 8*3*3,
                          "quat_t" : 8*4}


class KeyMode(IntEnum):
    NORMAL = 0,
    MATH = auto()


class ParseMode(IntEnum):
    NONE = 0
    HEADER = auto()
    LEFT_BRKT = auto()
    RIGHT_BRKT = auto()
    TYPEDEF = auto()

class ParseDeclMode(IntEnum):
    NONE = 0
    HEADER = auto()
    LEFT_BRKT = auto()
    RIGHT_BRKT = auto()
    DECL_TYPE = auto()
    DECL_NAME = auto()
    DECL_END  = auto()

class MemDefInf:
    def __init__(self, name:str, type:str):
        """
        Description: 
            コンストラクタ
        Param:          
            name: str
                構造体のメンバの名前
            type: str
                構造体のメンバの型
        """        
        self._name = name
        self._type = type
    def __copy__(self):
        """
        Description:
            コピーコンストラクタ
        """        
        return MemDefInf(self._type, self._members)
    
class StructDefInf:
    def __init__(self, type: str, members: list[MemDefInf]):
        """
        Description:
            コンストラクタ
        Param:          
            type: str
                構造体の型
            members: class
                構造体のメンバ
        """        
        self._type = type
        self._members = members
    def __copy__(self):
        """
        Description:
            コピーコンストラクタ
        """        
        return StructDefInf(self._type, self._members)



class StructDefInfMap:
    def __init__(self, sd_list: list[StructDefInf]):
        """
        Description:
            コンストラクタ
        Param:          
            sd_list: list
                構造体のリスト
       """        
        self._sd_list = sd_list
    def __copy__(self):
        """
        Description:
            コピーコンストラクタ
        """
        return StructDefInfMap(self._sd_list)
    def __iadd__(self, other):
        """
        Description:
            +=の算術演算子
        """
        if type(other) == StructDefInfMap:
            self._sd_list +=  other._sd_list
            return self        

    def get_type_list(self):
        """
        Description:
            型のリストをかえす
        Param:          
            なし
        Return:
            type_list : list
                型のリスト            
        """
        type_list = []
        for sd in self._sd_list:
            type_list.append(sd._type)
        return type_list
    
    def get_members(self, mode:KeyMode, type:str, parent_name:str, mem_list:list[list[str]]):
        """
        Description:
            指定された構造体のメンバのリストを返す
        Param:          
            type : str 
                指定された構造体の型
            parent_name : str
                親のメンバの名前
            mem_list : list
                構造体のメンバのリスト
        Return:
            mem_list : list
                構造体のメンバのリスト            
        """


        if mode == KeyMode.NORMAL:
            type_keys = element_type_keys
        else:    
            type_keys = element_type_keys_math

        temp_mem_list = []    
        for sd in self._sd_list:
            if (sd._type == type):
                if (type in type_keys == True):
                    temp_mem_list.append([sd._name, sd._type])
                else:
                    for member in sd._members:
                        if len(parent_name) == 0:
                            temp_name = member._name
                        else:
                            temp_name = parent_name + "." + member._name

                        if ((member._type in type_keys) == True):
                            temp_mem_list.append([temp_name, member._type])
                        else:    
                            temp_mem_list += self.get_members(mode, member._type, temp_name, mem_list)

        return mem_list + temp_mem_list             
    
    def parse_struct(self, words:list[str]):
        """
        Description:
            構文解析して構造体のデータを作成する
        Param:
            words: list
                単語のlist
        Return:
            なし
        """
        header = "struct"
        left_bracket = "{"
        right_bracket = "}"
        semi_colums = ";"
        mode = ParseMode.HEADER

        self._sd_list = []
        mem_type_flag = False
        mem_init_flag = False
        for word in words:
            if (mode == ParseMode.HEADER): # ヘッダが見つかるまで検索
                if (word == header):
                    mode =  ParseMode.LEFT_BRKT
            elif (mode == ParseMode.LEFT_BRKT): # {が見つかるまで検索
                if (word == left_bracket):
                    mode = ParseMode.RIGHT_BRKT
                    sd_temp = StructDefInf("", [])
                    
            elif (mode == ParseMode.RIGHT_BRKT): # }が見つかるまでメンバのtypeとnameを取得
                if (word == right_bracket):
                    mode = ParseMode.TYPEDEF
                else:
                    if (mem_init_flag == False):
                        mem_temp = MemDefInf("", "")
                        mem_init_flag = True
                        mem_type_flag = False

                    if word == semi_colums: # ;がみつかったら終了
                        mem_type_flag = False    
                    elif mem_type_flag == False:
                        if((word in element_type_keys) == True):
                            mem_temp._type = word
                        else:
                            mem_temp._type = word
                        mem_type_flag = True        
                    else:
                        mem_temp._name = word
                        sd_temp._members.append(mem_temp)
                        mem_init_flag = False
                        
            elif (mode == ParseMode.TYPEDEF): # typedefの名称が見つかるまで検索
                mode = ParseMode.HEADER
                sd_temp._type = word
                self._sd_list.append(sd_temp)
            else:
                pass     

    def dump(self):
        """
        Description:
            データをダンプする
        """
        print ("----------------------")
        for sd in self._sd_list:
            print(sd._type)
            for member in sd._members:
                print("\t", member._type, member._name)

class ValInfo:
    def __init__(self, name:str, type:str, address:int):
        """
        Description:
            コンストラクタ
        """        
        self._name = name
        self._type = type
        self._address = address
    def __copy__(self):
        """
        Description:
            コピーコンストラクタ
        """        
        return ValInfo(self._name, self._type, self._address)    

class ValInfoMap:
    def __init__(self, v_list:list[ValInfo]):
        """
        Description:
            コンストラクタ
        """          
        self._v_list = v_list
    def __copy__(self):
        """
        Description:
            コピーコンストラクタ
        """          
        return ValInfoMap(self._v_list)    
    def __iadd__(self, other):
        """
        Description:
            +=の算術演算子
        """
        if type(other) == ValInfoMap:
            self._v_list +=  other._v_list
            return self
                  
    def parse_struct_decl(self, words:list[str], type_list:list[str]):
        """
        Description:
            構文解析して構造体の宣言を抽出する
        Param:
            words: list
                単語のlist
            sd_list: list
                構造体情報のlist
        Return:
            なし
        """     
        
        self._v_list = []
        v_temp = ValInfo("", "", 0)
        header = "struct"
        left_bracket = "{"
        right_bracket = "}"
        semi_colums = ";"

        for _type in type_list:
            mode = ParseDeclMode.NONE
            for word in words:
                if mode == ParseDeclMode.NONE: 
                    if left_bracket == word:
                        mode = ParseDeclMode.RIGHT_BRKT    
                    elif _type == word:
                        mode = ParseDeclMode.DECL_NAME
                        v_temp._type = word
                elif mode == ParseDeclMode.RIGHT_BRKT:
                    if right_bracket == word:
                        mode = ParseDeclMode.NONE
                        v_temp._type = ""
                        v_temp._name = ""
                elif mode == ParseDeclMode.DECL_NAME:
                    if word == semi_colums or ("(" in word) == True or (")" in word) == True :
                        mode = ParseDeclMode.NONE
                        v_temp._type = ""
                        v_temp._name = ""
                    else:
                        v_temp._name = word
                        mode = ParseDeclMode.DECL_END
                elif mode == ParseDeclMode.DECL_END:
                    if word == semi_colums:
                        self._v_list.append(v_temp)
                        v_temp = ValInfo("", "", 0)
                        mode = ParseDeclMode.NONE
                    else:
                        mode = ParseDeclMode.NONE
                        v_temp._type = ""
                        v_temp._name = ""

    def dump(self):
        """
        Description:
            データをダンプする
        """
        print ("----------------------")
        for v in self._v_list:
            print("\t", v._type, v._name, v._address)

class SymbolInf:
    def __init__(self, name:str, address:int, type:str):
        """
        Description:
            コンストラクタ
        """
        self._name = name
        self._address = address
        self._type = type
    def __copy__(self):
        """
        Description:
            コピーコンストラクタ
        """
        return SymbolInf(self._name, self._address, self._type)    

class SymbolInfoMap:
    def __init__(self, smb_list:list[SymbolInf]):
        """
        Description:
            コンストラクタ
        """        
        self._smb_list = smb_list
        self._align = 8 # アライメントを8とする        
    def __copy__(self):
        """
        Description:
            コピーコンストラクタ
        """        
        return SymbolInfoMap(self._smb_list)    
    def __iadd__(self, other):
        """
        Description:
            +=の算術演算子
        """
        if type(other) == SymbolInfoMap:
            self._smb_list +=  other._smb_list
            return self          

    def parse_symbol(self, mode:KeyMode, sd_map:StructDefInfMap, val_map:ValInfoMap):
        """
        Description:
            シンボルデータを作成する
        Param:
            sd_map: StructDefInfMap
                構造体情報のMAP
            v_map: ValInfoMap
                アクセス対象の変数のMAP    
        """
        self._smb_list = []
        st_mem_dict = {}

        if mode == KeyMode.NORMAL:
            type_keys = element_type_keys
        else:    
            type_keys = element_type_keys_math

        for v in val_map._v_list:
            if (v._type in type_keys) == True:
                pass 
            else:
                mem_list = []
                mem_list = sd_map.get_members(mode, v._type, "", mem_list)
                st_mem_dict[v._type] = mem_list

        print(st_mem_dict)

        for v in val_map._v_list:
            if (v._type in type_keys) == True:
                smb = SymbolInf(v._name, v._address, v._type)
                self._smb_list.append(smb)
            else:
                offset = 0
                for temp_mem_list in st_mem_dict[v._type]:
                    mem_name = temp_mem_list[0]
                    mem_type = temp_mem_list[1]
                    if mem_type == "double" and (v._address + offset)%self._align > 0:
                        # 構造体のdoubleは8byteの境界で整列する
                        padding = self._align - (v._address + offset)%self._align
                        offset += padding
                    smb = SymbolInf(v._name + '.' + mem_name, v._address + offset, mem_type)
                    self._smb_list.append(smb)
                    offset += element_type_size_dict[mem_type]


    def dump(self):
        """
        Description:
            データをダンプする
        """
        print ("----------------------")
        for smb in self._smb_list:
            print("{} {} {:#10x}".format(smb._name, smb._type, smb._address))


def write_symbolmap_dump(object_file_path:str, symbolmap_file_path:str):
    """
    Description:
        nmコマンドを実行してsymbol mapを出力する
    Param:
        object_file_path: str
            オブジェクトファイルのパス
        symbolmap_file_path: str
            nmコマンドの出力ファイルのパス
    Return:
        なし
    """

    # コマンドを実行
    result = subprocess.run(['nm', object_file_path], capture_output=True, text=True)

    # 出力結果を取得
    output = result.stdout

    #path = 'symbol_map_temp.txt'
    with open(symbolmap_file_path, mode='w') as f:
        f.write(output)

def read_symbolmap_dump(symbolmap_file_path:str, v_map:ValInfoMap):
    """
    Description:
        アクセス対象の変数のMAPにアドレスの情報を反映する
    Param:
        symbolmap_file_path: str
            nmコマンドの出力ファイルのパス
        v_map: ValInfoMap
            アクセス対象の変数のMAP    
    Return:
        v_map: ValInfoMap
            アクセス対象の変数のMAP    
    """

    # nmコマンドの出力結果読み込み
    encoding = 'shift-jis'  
    colspecs = [
        (0, 16),   # アドレス
        (16, 18),  # タイプ
        (18, 256)  # オブジェクト名
    ]
    df = pd.read_fwf(
        symbolmap_file_path,
        colspecs=colspecs,
        encoding=encoding,
        errors='ignore',  # 読み込みエラーを無視 (必要に応じて設定)
        header=None,
        names=['address','type','name'],
        index_col=None
    )

    # シンボル検索
    for v in v_map._v_list:
        df1 = df[df['name'] == v._name]
        v._address = int(df1['address'].values[0], 16)

    return v_map


def open_cfile(path:str):
    """
    Description:
        ヘッダファイルを単語ごとに分解する
    Param:
        path: str
            対象のパス
    Return:
        words: list
            単語のlist
    """
    words = []
    with open(path, 'r') as f:
        for line in f:
            temp_words = []    
            line_words = line.replace(';', ' ;').split()
            for word in line_words:
                if word[:2] == "//":
                    break
                else:
                    temp_words.append(word)

            words += temp_words
    return words

def hex_representer(dumper, data):
    """
    Description:
        yamlの数値の出力を16進に変更する
    Param:
        dumper: object
            yamlをダンプするobject
        data: int
            出力データ    
    Return:
        dumper: object
    """       
    return dumper.represent_scalar('tag:yaml.org,2002:int', f'0x{data:08X}')

def write_symbolmap_yaml(path:str, smb_map:SymbolInfoMap):
    """
    Description:
        symbol_mapのyamlファイルを出力する
    Param:
        path: str
            symbol_mapのyamlファイルのパス
        smb_map: SymbolInfoMap
            シンボルマップの情報のMAP    
    Return:
        なし
    """   

    # カスタムの表現方法を登録
    yaml.add_representer(int, hex_representer)

    # 初期化
    symbol_map_dic = {}
    symbols_dic = {}
    symbols_list = []

    # Header出力
    symbol_map_dic['title'] = "symbol map"
    symbol_map_dic['description'] = "This is auto generated symbol map of Avisyslator GNC Prototype."
    for smb in smb_map._smb_list:
        if (smb._name == "mark_variable"):
            symbol_map_dic['mark_address'] = smb._address
        else:       
            symbols_dic = {}
            symbols_dic["name"]   = smb._name 
            symbols_dic["address"] = smb._address
            symbols_dic["type"]   = smb._type
            symbols_list.append(symbols_dic)
    symbol_map_dic['symbols'] = symbols_list
    with open(path, 'w') as file:
        yaml.dump(symbol_map_dic, file,
                   encoding='utf-8',
                     allow_unicode = True,
                     indent = 4,
                     sort_keys = False)



def create_symbolmap_yaml(hdr_path_list:list[str], obj_path:str, sybl_map_path:str):
    """
    Description:
        symbol_mapのyamlファイル作成する
    Param:
        file_path_list: list
            symbol_map出力のヘッダ
        obj_file: str
            オブジェクトファイル名
        sybl_map_path: str
            出力先のファイル名
    Return:
        なし
    """   
    sybl_temp_path = "symbol_map_temp.txt"
    sd_map = StructDefInfMap([])
    v_map = ValInfoMap([])
    
    for path in hdr_path_list:
        # 指定したファイルを読み込む
        words = open_cfile(path)

        # 指定したファイルから構造体を解析する
        sd_map_temp = StructDefInfMap([])
        sd_map_temp.parse_struct(words)
        type_list = sd_map_temp.get_type_list()

        # 構造体の宣言を解析する
        v_map_temp = ValInfoMap([])
        if ("ext.h" in path) == True:
            #print("e", path)
            v_map_temp.parse_struct_decl(words, type_list) 
        else:
            #print("n", path)
            pass

        sd_map += sd_map_temp
        v_map  += v_map_temp

    # nmコマンドでsymbolmapを出力する
    write_symbolmap_dump(obj_path, sybl_temp_path)

    # symbolmapを読む
    v_map._v_list.append(ValInfo("mark_variable", "int32_t", 0))
    v_map = read_symbolmap_dump(sybl_temp_path, v_map)

    mode = KeyMode.NORMAL
    smb_map = SymbolInfoMap([])
    smb_map.parse_symbol(mode, sd_map, v_map)
    smb_map.dump()
 
    mode = KeyMode.MATH
    smb_map_math = SymbolInfoMap([])
    smb_map_math.parse_symbol(mode, sd_map, v_map)
    smb_map_math.dump()

    smb_map += smb_map_math
 
    # symbolmapをyamlで書く
    write_symbolmap_yaml(sybl_map_path, smb_map)

if __name__ == '__main__':
    obj_file = sys.argv[1]
    file_path_list = sys.argv[2:]
    sybl_map_path = "symbol_map.yaml"

    create_symbolmap_yaml(file_path_list, obj_file, sybl_map_path)
